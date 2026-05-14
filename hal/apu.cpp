#include "../kernel/util.hpp"
#include "hal.hpp"

#ifdef SB_SUPPORT
#include "soundblaster.hpp"
#endif

namespace HAL {

// ========================================================================
// APU Implementation
// ========================================================================

#ifdef EXPERIMENTAL_PC_SPEAKER
// PC Speaker State (Square 2 only)
static u16 sq2_period = 0;
static u8 sq2_vol = 0;          // 0 = mute, >0 = play
static bool sq2_enabled = true; // From $4015

static void pc_speaker_set(u16 frequency) {
  if (frequency < 20 || frequency > 20000)
    return;
  u32 divisor = 1193182 / frequency;

  // Set PIT Channel 2 Mode 3 (Square Wave)
  outb(0x43, 0xB6);
  outb(0x42, (u8)(divisor & 0xFF));
  outb(0x42, (u8)((divisor >> 8) & 0xFF));

  // Enable Speaker (Bits 0+1 of 0x61)
  u8 tmp = inb(0x61);
  if (tmp != (tmp | 3)) {
    outb(0x61, tmp | 3);
  }
}

static void pc_speaker_mute() {
  u8 tmp = inb(0x61);
  outb(0x61, tmp & 0xFC); // Clear bits 0-1
}

static void update_sq2() {
  if (!sq2_enabled || sq2_vol == 0 || sq2_period < 8) {
    pc_speaker_mute();
    return;
  }

  // NES Frequency Formula: CPU / (16 * (P + 1))
  // P = sq2_period
  // CPU approx 1789773 Hz
  // F = 111861 / (sq2_period + 1)

  u32 freq = 111861 / (sq2_period + 1);
  pc_speaker_set((u16)freq);
}
#endif

void apu_write(u16 addr, u8 val) {
#ifdef SB_SUPPORT
  HAL::SB::write_apu(addr, val);
#endif
#ifdef EXPERIMENTAL_PC_SPEAKER
  // Square 2 Registers ($4004 - $4007)
  if (addr == 0x4004) {
    // Duty/Volume
    // Bits 0-3: Volume/Envelope
    // Bit 4: Envelope Disable (Constant Volume) -> we treat any non-zero V as
    // ON
    sq2_vol = val & 0x0F;
    if ((val & 0x10) == 0) { // Constant volume disabled check?
                             // Actually NES logic is complex.
      // Simple approximation: If Vol>0 or ConstantVol flag set with Vol>0...
      // Let's just use low 4 bits as strict volume for now.
    }
    // Better: If (Bit 4 set) Vol = Bits 0-3. Else Vol = Envelope (assume max
    // for now if not emulating) Sticking to simple: Vol = val & 0x0F;
    update_sq2();
  } else if (addr == 0x4006) {
    sq2_period = (sq2_period & 0xFF00) | val;
  } else if (addr == 0x4007) {
    // TTTT LLLL (Timer High, Length Counter)
    sq2_period = (sq2_period & 0x00FF) | ((u16)(val & 0x7) << 8);
    // Length counter load = val >> 3. Not emulating length counter yet (PC
    // Speaker is persistent).
    update_sq2();
  } else if (addr == 0x4015) {
    // Status: Enable/Disable channels
    // Bit 1: Square 2
    sq2_enabled = (val & 0x02) != 0;
    update_sq2();
  }
#else
  (void)addr;
  (void)val;
#endif
}

u8 apu_read(u16 addr) {
  // $4015 Status stub
  (void)addr;
  return 0;
}
} // namespace HAL
