#include "../kernel/util.hpp"
#include "hal.hpp"
#ifdef GD5428_OPTIMIZE
#include "gd5428.hpp"
#endif
// Tick counter from idt.cpp (for FPS display)
extern "C" volatile u32 g_tick_count;

namespace HAL {

// ========================================================================
// Rendering Implementation
// ========================================================================

struct BootInfo {
  u32 signature;     // 'SMBr'
  u32 phys_base_ptr; // Framebuffer physical address
  u16 bytes_per_line;
  u16 x_res;
  u16 y_res;
  u8 bits_per_pixel;
  u8 mode_flag; // 0 = Linear (VESA/13h), 1 = Mode X (planar)
};

static BootInfo *boot_info = (BootInfo *)0x9000;

// Mode X plane selection (via VGA Sequencer)
static inline void set_write_plane(u8 plane) {
  outb(0x3C4, 0x02);       // Sequencer: Map Mask Register
  outb(0x3C5, 1 << plane); // Select plane (0-3)
}

static inline void set_all_planes() {
  outb(0x3C4, 0x02); // Sequencer: Map Mask Register
  outb(0x3C5, 0x0F); // All 4 planes
}

// Mode X pixel write (320x240 planar)
// Each row is 80 bytes (320 / 4 planes)
// Pixel X goes to plane (X % 4), offset (X / 4)
static void blit_to_modex(u8 *src, u8 *vram, int src_width, int src_height,
                          int start_x) {
  // For each plane, write every 4th pixel
  for (int plane = 0; plane < 4; plane++) {
    set_write_plane(plane);

    for (int y = 0; y < src_height; y++) {
      u8 *src_row = &src[y * src_width];
      u8 *dst_row = &vram[y * 80]; // 80 bytes per row in Mode X

      // Starting X position offset for this plane
      int x_offset = (start_x + plane) % 4;
      int dst_start = (start_x + plane) / 4;

      // Write pixels for this plane
      for (int x = plane; x < src_width; x += 4) {
        int screen_x = start_x + x;
        int dst_offset = screen_x / 4;
        dst_row[dst_offset] = src_row[x];
      }
    }
  }

  set_all_planes(); // Restore to all planes
}

void render() {
  if (boot_info->signature != 0x72424D53)
    return;

  u8 *vram = (u8 *)boot_info->phys_base_ptr;
  u32 screen_width = boot_info->x_res;
  u32 screen_height = boot_info->y_res;

  // Drive PPU render
  ppu_render_frame(0);

  // Get source
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

  // Draw FPS at top-right (NES buffer coordinates)
  int fx = 100; // Shifted left to fit 5 digits (256 - 24)
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

#ifdef SCALE_2X
  // Scaled mode: 512x480 centered in 640x480
  // OPTIMIZED: Write 2 source pixels (4 dest pixels) per 32-bit write
  u32 start_x = 64;
  u32 start_y = 0;

  for (int y = 0; y < 240; y++) {
    u8 *src_row = &src[y * 256];
    u32 *dst_row1 = (u32 *)&vram[(start_y + y * 2) * screen_width + start_x];
    u32 *dst_row2 =
        (u32 *)&vram[(start_y + y * 2 + 1) * screen_width + start_x];

    // Process 2 source pixels at a time (4 dest pixels = 1 u32)
    for (int x = 0; x < 256; x += 2) {
      u8 p0 = src_row[x];
      u8 p1 = src_row[x + 1];
      // Pack: [p0, p0, p1, p1] into a u32 (little-endian)
      u32 packed = (u32)p0 | ((u32)p0 << 8) | ((u32)p1 << 16) | ((u32)p1 << 24);
      dst_row1[x / 2] = packed;
      dst_row2[x / 2] = packed;
    }
  }
#else
  // Check video mode by resolution
  if (screen_width == 320 && screen_height == 240 &&
      boot_info->mode_flag == 1) {
    // ===========================================
    // Mode X: 320x240 planar - Perfect for NES!
    // ===========================================
    // Center horizontally: (320 - 256) / 2 = 32 pixels
    blit_to_modex(src, vram, 256, 240, 32);

  } else if (screen_width == 320 && screen_height == 200) {
    // ===========================================
    // Mode 13h: 320x200 linear
    // ===========================================
    u32 start_x = 32;
    u32 crop_top = 20;

#ifdef GD5428_OPTIMIZE
    if (GD5428::is_available()) {
      // NOTE: Hardware BitBLT (GD542x) requires entering an Extended Linear mode (SR7 bit 0).
      // Entering this mode fundamentally overrides VGA Pixel Multiplexing (Chain-4), 
      // inadvertently forcing the CRTC to display raw 640x480 resolution timings.
      // This is what caused the "two Marios side-by-side" artifact. 
      // Since 'rep movsb' on a VLB 486 can easily saturate bus speeds and exceed ~600 FPS
      // without needing hardware assistance, we fully bypass the BitBLT routing here.
    }
#endif

    // Software Rendering utilizing optimized VLB 'rep movsb' saturation.
    for (int y = 0; y < 200; y++) {
      void *src_row = (void *)&src[(crop_top + y) * 256];
      void *dst_row = (void *)&vram[y * 320 + start_x];
      u32 cnt = 256;
      asm volatile(
          "rep movsb"
          : "+D"(dst_row), "+S"(src_row), "+c"(cnt)
          :
          : "memory"
      );
    }
  } else {
    // ===========================================
    // VESA 101h: 640x480 linear
    // ===========================================
    u32 start_x = (screen_width - 256) / 2;
    u32 start_y = (screen_height - 240) / 2;

    for (int y = 0; y < 240; y++) {
      u8 *src_row = &src[y * 256];
      u8 *dst_row = &vram[(start_y + y) * screen_width + start_x];
      memcpy(dst_row, src_row, 256);
    }
  }
#endif
}
} // namespace HAL
