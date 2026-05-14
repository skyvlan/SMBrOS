// ============================================================================
// hal/gd5428.cpp — Cirrus Logic CL-GD5428 BitBLT Hardware Acceleration
// ============================================================================
// Provides hardware-accelerated VRAM operations via the GD5428 BitBLT engine.
// All functions are compiled only when GD5428_OPTIMIZE is defined.
//
// Register reference (Graphics Controller, port 3CE/3CF):
//   GR20-21: BLT Width  (pixels - 1)
//   GR22-23: BLT Height (lines - 1)
//   GR24-25: Dest Pitch
//   GR26-27: Source Pitch
//   GR28-2A: Dest Address (22-bit)
//   GR2C-2E: Source Address (22-bit)
//   GR30:    BLT Mode (bit 0=direction, bit 2=host source, bit 7=color expand)
//   GR31:    BLT Start/Status (write bit 1=start, read bit 3=busy)
//   GR32:    ROP (0x0D = source copy)
// ============================================================================

#ifdef GD5428_OPTIMIZE

#include "gd5428.hpp"
#include "../kernel/util.hpp"

namespace GD5428 {

// ========================================================================
// State
// ========================================================================
static bool detected = false;
static u8 chip_id = 0;

// ========================================================================
// Register Helpers
// ========================================================================

static inline void write_gr(u8 index, u8 val) {
  outb(0x3CE, index);
  outb(0x3CF, val);
}

static inline u8 read_gr(u8 index) {
  outb(0x3CE, index);
  return inb(0x3CF);
}

static inline void write_sr(u8 index, u8 val) {
  outb(0x3C4, index);
  outb(0x3C5, val);
}

static inline u8 read_sr(u8 index) {
  outb(0x3C4, index);
  return inb(0x3C5);
}

static inline u8 read_cr(u8 index) {
  outb(0x3D4, index);
  return inb(0x3D5);
}

static inline void write_cr(u8 index, u8 val) {
  outb(0x3D4, index);
  outb(0x3D5, val);
}

// ========================================================================
// Detection
// ========================================================================

bool init() {
  // Step 1: Unlock Cirrus extensions (SR06 = 0x12)
  write_sr(0x06, 0x12);
  u8 sr06 = read_sr(0x06);

  if (sr06 != 0x12) {
    // Not a Cirrus Logic chip — lock sequence didn't work
    detected = false;
    return false;
  }

  // Step 2: Read chip ID from CR27
  chip_id = read_cr(0x27);

  // Accept GD5426/28/29/30/34 — all have BitBLT engines
  // CR27 upper 6 bits = chip family, lower 2 = revision
  // GD5426: ~0x90, GD5428: ~0x94-0x98, GD5429: ~0x9C
  // GD5430: ~0xA0, GD5434: ~0xB8
  u8 family = chip_id & 0xFC;
  if (family >= 0x90 && family <= 0xBC) {
    detected = true;
    serial_puts("[GD5428] Detected Cirrus Logic chip ID=0x");
    serial_puthex(chip_id, 2);
    serial_puts("\r\n");
  } else {
    detected = false;
    write_sr(0x06, 0x00); // Re-lock extensions
    serial_puts("[GD5428] Not detected (CR27=0x");
    serial_puthex(chip_id, 2);
    serial_puts(")\r\n");
  }

  return detected;
}

bool is_available() { return detected; }

void enable_linear_packed_mode() {
  if (!detected) return;
  // Unlock extended registers
  write_sr(0x06, 0x12);
  
  // SR7: Extended Sequencer Mode
  // Bit 0 = Extended 256-color map (disables VGA pixel merging, enables true linear packing)
  u8 sr7 = read_sr(0x07);
  write_sr(0x07, sr7 | 0x01);

  serial_puts("[GD5428] Enabled True Linear Packed-Pixel Mode via SR7.\r\n");
}

// ========================================================================
// BitBLT Engine
// ========================================================================

void wait_idle() {
  // GR31 bit 3 = BLT busy (per Linux cirrusfb driver)
  while (read_gr(0x31) & 0x08)
    ;
}

void setup_host_blt(u32 dest_offset, u16 width, u16 height,
                    u16 dest_pitch) {
  wait_idle();

  u16 w = width - 1;
  u16 h = height - 1;

  // Reset BLT engine
  write_gr(0x31, 0x04);

  // BLT dimensions
  write_gr(0x20, w & 0xFF);
  write_gr(0x21, (w >> 8) & 0x1F);
  write_gr(0x22, h & 0xFF);
  write_gr(0x23, (h >> 8) & 0x07);

  // Destination pitch
  write_gr(0x24, dest_pitch & 0xFF);
  write_gr(0x25, (dest_pitch >> 8) & 0x0F);

  // Source pitch (match pixel width for contiguous host data)
  write_gr(0x26, width & 0xFF);
  write_gr(0x27, (width >> 8) & 0x0F);

  // Destination address (22-bit VRAM offset)
  write_gr(0x28, dest_offset & 0xFF);
  write_gr(0x29, (dest_offset >> 8) & 0xFF);
  write_gr(0x2A, (dest_offset >> 16) & 0x3F);

  // ROP: source copy
  write_gr(0x32, 0x0D);

  // Mode: system memory source (host-to-screen), forward direction
  // Bit 2 = source from system memory
  write_gr(0x30, 0x04);

  // Start BLT — CPU now writes pixel data to VGA aperture (0xA0000)
  write_gr(0x31, 0x02);
}

void screen_copy(u32 src_offset, u32 dest_offset, u16 width, u16 height,
                 u16 pitch) {
  wait_idle();

  u16 w = width - 1;
  u16 h = height - 1;

  // Determine direction for overlapping copies
  u8 mode = 0x00; // Forward (top-to-bottom, left-to-right)
  u32 src_addr = src_offset;
  u32 dst_addr = dest_offset;

  if (dest_offset > src_offset) {
    // Backward copy needed (bottom-to-top, right-to-left)
    mode = 0x01;
    src_addr += (u32)(height - 1) * pitch + (width - 1);
    dst_addr += (u32)(height - 1) * pitch + (width - 1);
  }

  // Reset BLT
  write_gr(0x31, 0x04);

  // Dimensions
  write_gr(0x20, w & 0xFF);
  write_gr(0x21, (w >> 8) & 0x1F);
  write_gr(0x22, h & 0xFF);
  write_gr(0x23, (h >> 8) & 0x07);

  // Pitches (same for src and dest)
  write_gr(0x24, pitch & 0xFF);
  write_gr(0x25, (pitch >> 8) & 0x0F);
  write_gr(0x26, pitch & 0xFF);
  write_gr(0x27, (pitch >> 8) & 0x0F);

  // Source address
  write_gr(0x2C, src_addr & 0xFF);
  write_gr(0x2D, (src_addr >> 8) & 0xFF);
  write_gr(0x2E, (src_addr >> 16) & 0x3F);

  // Destination address
  write_gr(0x28, dst_addr & 0xFF);
  write_gr(0x29, (dst_addr >> 8) & 0xFF);
  write_gr(0x2A, (dst_addr >> 16) & 0x3F);

  // ROP: source copy
  write_gr(0x32, 0x0D);

  // Mode (direction)
  write_gr(0x30, mode);

  // Start
  write_gr(0x31, 0x02);
}

} // namespace GD5428

#endif // GD5428_OPTIMIZE
