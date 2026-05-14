/**
 * SMBEngine.cpp - Implementation of SMB engine helper methods
 */
#include "../hal/hal.hpp"
#include "SMB.hpp"

// Extern from smb_chr.cpp
extern u8 *get_chr_pointer();

//---------------------------------------------------------------------
// MemoryAccess Implementation
//---------------------------------------------------------------------

void MemoryAccess::set(uint8_t v) {
  if (!is_const) {
    *ptr = v;
  }
  // Update flags
  engine.z = (v == 0);
  engine.n = (v & 0x80) != 0;
}

MemoryAccess &MemoryAccess::operator=(uint8_t v) {
  set(v);
  return *this;
}

MemoryAccess &MemoryAccess::operator=(const MemoryAccess &rhs) {
  set(rhs.is_const ? rhs.const_val : *rhs.ptr);
  return *this;
}

MemoryAccess &MemoryAccess::operator+=(uint8_t v) {
  uint16_t result = get() + v + (engine.c ? 1 : 0);
  engine.c = (result > 0xff);
  set(static_cast<uint8_t>(result));
  return *this;
}

MemoryAccess &MemoryAccess::operator-=(uint8_t v) {
  uint16_t result = get() - v - (engine.c ? 0 : 1);
  engine.c = (result <= 0xff); // Borrow flag (inverted)
  set(static_cast<uint8_t>(result));
  return *this;
}

MemoryAccess &MemoryAccess::operator++() {
  uint8_t val = get() + 1;
  set(val);
  engine.z = (val == 0);
  engine.n = (val & 0x80) != 0;
  return *this;
}

MemoryAccess &MemoryAccess::operator--() {
  uint8_t val = get() - 1;
  set(val);
  engine.z = (val == 0);
  engine.n = (val & 0x80) != 0;
  return *this;
}

MemoryAccess &MemoryAccess::operator++(int) {
  uint8_t val = get() + 1;
  set(val);
  engine.z = (val == 0);
  engine.n = (val & 0x80) != 0;
  return *this;
}

MemoryAccess &MemoryAccess::operator--(int) {
  uint8_t val = get() - 1;
  set(val);
  engine.z = (val == 0);
  engine.n = (val & 0x80) != 0;
  return *this;
}

MemoryAccess &MemoryAccess::operator&=(uint8_t v) {
  set(get() & v);
  return *this;
}

MemoryAccess &MemoryAccess::operator|=(uint8_t v) {
  set(get() | v);
  return *this;
}

MemoryAccess &MemoryAccess::operator^=(uint8_t v) {
  set(get() ^ v);
  return *this;
}

MemoryAccess &MemoryAccess::operator<<=(int shift) {
  uint8_t val = get();
  engine.c = (val & 0x80) != 0;
  set(val << shift);
  return *this;
}

MemoryAccess &MemoryAccess::operator>>=(int shift) {
  uint8_t val = get();
  engine.c = (val & 0x01) != 0;
  set(val >> shift);
  return *this;
}

MemoryAccess::operator uint8_t() {
  return get(); // Reference doesn't set flags on read - only LDA does
}

void MemoryAccess::rol() {
  uint8_t val = get();
  bool old_c = engine.c;
  engine.c = (val & 0x80) != 0;
  val = (val << 1) | (old_c ? 1 : 0);
  set(val);
}

void MemoryAccess::ror() {
  uint8_t val = get();
  bool old_c = engine.c;
  engine.c = (val & 0x01) != 0;
  val = (val >> 1) | (old_c ? 0x80 : 0);
  set(val);
}

//---------------------------------------------------------------------
// SMBEngine Implementation
//---------------------------------------------------------------------

SMBEngine::SMBEngine()
    : a(*this, &registerA), x(*this, &registerX), y(*this, &registerY),
      s(*this, &registerS), c(false), z(false), n(false), registerA(0),
      registerX(0), registerY(0), registerS(0xff), returnIndexStackTop(0),
      controller1_state(0), controller2_state(0), chr(nullptr),
      initializing(true) {
  // Clear memory
  for (int i = 0; i < 0x800; i++)
    ram[i] = 0;
  for (int i = 0; i < 0x8000; i++)
    dataStorage[i] = 0;
}

void SMBEngine::reset() {
  initializing = true;
  returnIndexStackTop = 0;
  registerS = 0xff;
  chr = get_chr_pointer(); // Load CHR ROM
  code(0);                 // Run initialization
  initializing = false;    // Init done, run normal PPU logic
}

void SMBEngine::update() {
  code(1); // Run frame logic (NMI handler)
}

void SMBEngine::render(uint32_t *buffer) {
  // TODO: Integrate with HAL::render()
}

void SMBEngine::compare(uint8_t val1, uint8_t val2) {
  int result = val1 - val2;
  c = (val1 >= val2);
  z = (val1 == val2);
  n = (result & 0x80) != 0;
}

void SMBEngine::bit(uint8_t val) {
  uint8_t result = registerA & val;
  z = (result == 0);
  n = (val & 0x80) != 0;
  // Note: BIT also sets V flag but we don't use it
}

uint8_t *SMBEngine::getDataPointer(uint16_t addr) {
  if (addr < 0x800) {
    return &ram[addr];
  } else if (addr < 0x2000) {
    return &ram[addr & 0x7ff]; // RAM mirrors
  } else if (addr >= 0x8000) {
    return &dataStorage[addr - 0x8000];
  }
  // For I/O addresses, return nullptr
  return nullptr;
}

MemoryAccess SMBEngine::getMemory(uint16_t addr) {
  uint8_t *ptr = getDataPointer(addr);
  if (ptr) {
    return MemoryAccess(*this, ptr);
  }
  // For I/O registers, handle specially
  if (addr >= 0x2000 && addr < 0x4020) {
    uint8_t val = readPPU(addr);
    return MemoryAccess(*this, val);
  }
  return MemoryAccess(*this, (uint8_t)0);
}

uint16_t SMBEngine::getMemoryWord(uint8_t addr) {
  // Zero page word read with wrap-around
  uint8_t lo = ram[addr];
  uint8_t hi = ram[(addr + 1) & 0xff];
  return lo | (hi << 8);
}

void SMBEngine::writeData(uint16_t addr, uint8_t val) {
  if (addr < 0x800) {
    ram[addr] = val;
  } else if (addr < 0x2000) {
    ram[addr & 0x7ff] = val;
  } else if (addr >= 0x2000 && addr < 0x4000) {
    writePPU(addr, val);
  } else if (addr >= 0x4000 && addr < 0x4020) {
    if (addr == 0x4016) {
      HAL::input_write(val); // Controller strobe
    } else if (addr == 0x4014) {
      // OAM DMA: Copy 256 bytes from RAM page (val * 256) to OAM
      u16 src_addr = (u16)val << 8;
      for (int i = 0; i < 256; i++) {
        u8 byte = ram[src_addr + i];
        HAL::ppu_write(0x2004, byte); // Write to OAMDATA
      }
    } else {
      writeAPU(addr, val);
    }
  } else if (addr >= 0x8000) {
    dataStorage[addr - 0x8000] = val;
  }
}

void SMBEngine::writeData(uint16_t addr, const uint8_t *data, size_t len) {
  for (size_t i = 0; i < len; i++) {
    writeData(addr + i, data[i]);
  }
}

void SMBEngine::pha() {
  ram[0x100 + registerS] = registerA;
  registerS--;
}

void SMBEngine::pla() {
  registerS++;
  a = ram[0x100 + registerS];
}

void SMBEngine::php() {
  uint8_t flags = (c ? 0x01 : 0) | (z ? 0x02 : 0) | (n ? 0x80 : 0) | 0x30;
  ram[0x100 + registerS] = flags;
  registerS--;
}

void SMBEngine::plp() {
  registerS++;
  uint8_t flags = ram[0x100 + registerS];
  c = (flags & 0x01) != 0;
  z = (flags & 0x02) != 0;
  n = (flags & 0x80) != 0;
}

// PPU/APU - hooks to HAL
uint8_t SMBEngine::readPPU(uint16_t addr) {
  // Handle controller reads ($4016, $4017)
  if (addr == 0x4016) {
    return HAL::input_read(0); // Controller 1
  } else if (addr == 0x4017) {
    return HAL::input_read(1); // Controller 2
  }

  uint8_t val = HAL::ppu_read(addr);

  // HACK: Force VBlank (bit 7) during initialization.
  // The game waits for 2 VBlanks at startup. Since our init is
  // blocking/single-threaded, we can't generate async VBlanks. Force it to
  // pass.
  if (initializing && addr == 0x2002) {
    val |= 0x80;
  }
  return val;
}

void SMBEngine::writePPU(uint16_t addr, uint8_t val) {
  HAL::ppu_write(addr, val);
}

void SMBEngine::writeAPU(uint16_t addr, uint8_t val) {
  HAL::apu_write(addr, val);
}

uint8_t SMBEngine::readController(int port) {
  return port == 0 ? controller1_state : controller2_state;
}
