/**
 * kernel_main.cpp - SMBrOS Kernel Entry Point
 *
 * This is the main C++ entry point called by crt0.asm after protected mode
 * initialization. It initializes the HAL and runs the SMB game loop.
 */

#include "../smb/SMB.hpp"
#include "common.hpp"
#include "types.h"
#include "util.hpp"

// Forward declarations - implemented in HAL modules
namespace HAL {
void init();
void init_timing(); // Initialize timer subsystem
void wait_vsync();
void render();
void ppu_set_vblank();
u8 *get_ppu_buffer(); // PPU internal buffer
} // namespace HAL

// External: tick counter from kernel/idt.cpp
extern "C" volatile u32 g_tick_count;

// Global SMB engine instance (used by HAL for rendering)
SMBEngine *g_smbEngine = nullptr;

#ifdef SHOW_FPS
// Simple 3x5 digit bitmaps for FPS display
static const u8 digit_bitmaps[10][5] = {
    {0x7, 0x5, 0x5, 0x5, 0x7}, // 0
    {0x2, 0x2, 0x2, 0x2, 0x2}, // 1
    {0x7, 0x1, 0x7, 0x4, 0x7}, // 2
    {0x7, 0x1, 0x7, 0x1, 0x7}, // 3
    {0x5, 0x5, 0x7, 0x1, 0x1}, // 4
    {0x7, 0x4, 0x7, 0x1, 0x7}, // 5
    {0x7, 0x4, 0x7, 0x5, 0x7}, // 6
    {0x7, 0x1, 0x1, 0x1, 0x1}, // 7
    {0x7, 0x5, 0x7, 0x5, 0x7}, // 8
    {0x7, 0x5, 0x7, 0x1, 0x7}, // 9
};

// Draw a digit at position (x, y) in the PPU buffer
static void draw_digit(u8 *buffer, int x, int y, int digit, u8 color) {
  if (digit < 0 || digit > 9)
    return;

  for (int row = 0; row < 5; row++) {
    u8 bits = digit_bitmaps[digit][row];
    for (int col = 0; col < 3; col++) {
      if (bits & (0x4 >> col)) {
        int px = x + col;
        int py = y + row;
        if (px >= 0 && px < 256 && py >= 0 && py < 240) {
          buffer[py * 256 + px] = color;
        }
      }
    }
  }
}

// Draw FPS counter (up to 3 digits + "FPS" label)
static void draw_fps(u8 *buffer, int fps) {
  // 6 digits * 4px width = 24px.
  // Screen width 256. 256 - 24 - 2 (margin) = 230.
  int x = 230;
  int y = 2;
  u8 color = 0x30; // White (NES palette)

  // Clamp FPS to 6 digits
  if (fps > 999999)
    fps = 999999;
  if (fps < 0)
    fps = 0;

  // Draw digits
  // We check divisors from 100,000 down to 10
  for (int divisor = 100000; divisor >= 10; divisor /= 10) {
    if (fps >= divisor) {
      draw_digit(buffer, x, y, (fps / divisor) % 10, color);
      x += 4;
    }
  }

  // Always draw the ones digit (even if 0)
  draw_digit(buffer, x, y, fps % 10, color);
}
#endif

/**
 * Main kernel entry point (called from crt0.asm)
 */
extern "C" void kernel_main() {
  // Initialize hardware abstraction layer
  HAL::init();

  // Initialize timing subsystem (needed for FPS counter, especially with
  // NO_VSYNC)
  HAL::init_timing();

  // Create and initialize SMB engine
  static SMBEngine engineInstance;
  g_smbEngine = &engineInstance;

  // Initialize game
  HAL::ppu_set_vblank();
  g_smbEngine->reset();

  // FPS tracking
  u32 frame_count = 0;
  u32 last_tick = g_tick_count;
  u32 fps = 0;

  // Main frame loop
  while (true) {
#ifndef NO_VSYNC
    HAL::wait_vsync();
#endif
    HAL::ppu_set_vblank(); // Set VBlank flag (simulates NMI timing)
    g_smbEngine->update(); // Run NMI handler (frame update)
    frame_count++;

#ifdef TEXT_MODE
    // TEXT_MODE: Skip rendering, write FPS to VGA text VRAM
    u32 current_tick = g_tick_count;
    if (current_tick - last_tick >= 60) {
      fps = frame_count;
      frame_count = 0;
      last_tick = current_tick;

      // Write "Logic FPS: XXX" to VGA text mode VRAM at 0xB8000
      // Each character is 2 bytes: [char][attribute]
      // Row 2, column 2 = offset (2 * 80 + 2) * 2 = 328
      volatile u16 *vram = (volatile u16 *)0xB8000;
      const char *label = "Logic FPS: ";
      int pos = 2 * 80 + 2; // Row 2, Col 2

      // Write label
      while (*label) {
        vram[pos++] = (u16)(0x0F00 | *label++); // White on black
      }

      // Write FPS digits (up to 5 digits for high FPS)
      u32 v = fps > 99999 ? 99999 : fps;
      if (v >= 10000)
        vram[pos++] = (u16)(0x0E00 | ('0' + (v / 10000) % 10)); // Yellow
      if (v >= 1000)
        vram[pos++] = (u16)(0x0E00 | ('0' + (v / 1000) % 10));
      if (v >= 100)
        vram[pos++] = (u16)(0x0E00 | ('0' + (v / 100) % 10));
      if (v >= 10)
        vram[pos++] = (u16)(0x0E00 | ('0' + (v / 10) % 10));
      vram[pos++] = (u16)(0x0E00 | ('0' + v % 10));
      vram[pos++] = (u16)(0x0F00 | ' '); // Clear trailing
      vram[pos++] = (u16)(0x0F00 | ' ');
    }
#else
#ifdef SHOW_FPS
    // Calculate FPS every ~60 ticks (1 second at 60Hz PIT)
    u32 current_tick = g_tick_count;
    if (current_tick - last_tick >= 60) {
      fps = frame_count;
      frame_count = 0;
      last_tick = current_tick;
    }

    // Draw FPS to buffer BEFORE render copies to VRAM
    draw_fps(HAL::get_ppu_buffer(), fps);
#endif

    HAL::render();
#endif
  }
}
