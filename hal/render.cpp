#include "../kernel/util.hpp"
#include "hal.hpp"

// Tick counter from idt.cpp (for FPS display)
extern "C" volatile u32 g_tick_count;

namespace HAL {

// ========================================================================
// Rendering Implementation — Mode 13h (320x200) only
// ========================================================================

struct BootInfo {
  u32 signature;     // 'SMBr'
  u32 phys_base_ptr; // Framebuffer physical address
  u16 bytes_per_line;
  u16 x_res;
  u16 y_res;
  u8 bits_per_pixel;
  u8 mode_flag; // 0 = Linear (Mode 13h)
};

static BootInfo *boot_info = (BootInfo *)0x9000;

void render() {
  if (boot_info->signature != 0x72424D53)
    return;

  u8 *vram = (u8 *)boot_info->phys_base_ptr;

  // Drive PPU render (dirty-tile aware)
  ppu_render_frame(0);

  // Get source buffer (256x240 indexed)
  u8 *src = get_ppu_buffer();

#ifdef SHOW_FPS
  // FPS counter - draw after PPU render, before VRAM copy
  static u32 frame_count = 0;
  static u32 last_tick = 0;
  static u32 fps = 0;

  frame_count++;
  u32 current_tick = g_tick_count;
  if (current_tick - last_tick >= 60) {
    fps = frame_count;
    frame_count = 0;
    last_tick = current_tick;
  }

  // Simple 3x5 digit bitmaps
  static const u8 digits[10][5] = {
      {0x7, 0x5, 0x5, 0x5, 0x7}, {0x2, 0x2, 0x2, 0x2, 0x2},
      {0x7, 0x1, 0x7, 0x4, 0x7}, {0x7, 0x1, 0x7, 0x1, 0x7},
      {0x5, 0x5, 0x7, 0x1, 0x1}, {0x7, 0x4, 0x7, 0x1, 0x7},
      {0x7, 0x4, 0x7, 0x5, 0x7}, {0x7, 0x1, 0x1, 0x1, 0x1},
      {0x7, 0x5, 0x7, 0x5, 0x7}, {0x7, 0x5, 0x7, 0x1, 0x7}};

  // Draw FPS at center of screen
  int fx = 100;
  int fy = 100;
  u8 color = 0x30; // White
  u32 fps_val = fps > 99999 ? 99999 : fps;

  // Draw up to 5 digits
  if (fps_val >= 10000) {
    int d = (fps_val / 10000) % 10;
    for (int r = 0; r < 5; r++)
      for (int c = 0; c < 3; c++)
        if (digits[d][r] & (4 >> c))
          src[(fy + r) * 256 + fx + c] = color;
    fx += 4;
  }
  if (fps_val >= 1000) {
    int d = (fps_val / 1000) % 10;
    for (int r = 0; r < 5; r++)
      for (int c = 0; c < 3; c++)
        if (digits[d][r] & (4 >> c))
          src[(fy + r) * 256 + fx + c] = color;
    fx += 4;
  }
  if (fps_val >= 100) {
    int d = (fps_val / 100) % 10;
    for (int r = 0; r < 5; r++)
      for (int c = 0; c < 3; c++)
        if (digits[d][r] & (4 >> c))
          src[(fy + r) * 256 + fx + c] = color;
    fx += 4;
  }
  if (fps_val >= 10) {
    int d = (fps_val / 10) % 10;
    for (int r = 0; r < 5; r++)
      for (int c = 0; c < 3; c++)
        if (digits[d][r] & (4 >> c))
          src[(fy + r) * 256 + fx + c] = color;
    fx += 4;
  }
  {
    int d = fps_val % 10;
    for (int r = 0; r < 5; r++)
      for (int c = 0; c < 3; c++)
        if (digits[d][r] & (4 >> c))
          src[(fy + r) * 256 + fx + c] = color;
  }
#endif

  // ===========================================
  // Mode 13h: 320x200 linear framebuffer
  // ===========================================
  // NES output is 256x240. Mode 13h is 320x200.
  // Center horizontally: (320 - 256) / 2 = 32 pixels
  // Crop 20 pixels from top/bottom: show rows 20-219 of the 240-line buffer
  u32 start_x = 32;
  u32 crop_top = 20;

  // Fast VRAM copy using rep movsd (DWORD) + rep movsb (remainder)
  // 256 bytes per row = 64 DWORDs exactly, no remainder needed
  for (int y = 0; y < 200; y++) {
    void *src_row = (void *)&src[(crop_top + y) * 256];
    void *dst_row = (void *)&vram[y * 320 + start_x];
    u32 dwords = 64; // 256 / 4 = 64 DWORDs
    asm volatile(
        "rep movsl"
        : "+D"(dst_row), "+S"(src_row), "+c"(dwords)
        :
        : "memory"
    );
  }
}
} // namespace HAL
