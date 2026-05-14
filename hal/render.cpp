#include "../kernel/util.hpp"
#include "hal.hpp"

// Tick counter from idt.cpp (for FPS display)
extern "C" volatile u32 g_tick_count;

namespace HAL {

// Expose draw_sprites_to_vram from video.cpp
void draw_sprites_to_vram(u8* vram, int start_x, int crop_top);
int get_scroll_x(); // Let's add this to video.cpp

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

  // Drive PPU render (dirty-tile aware, only draws to 512x240 internal buffer)
  ppu_render_frame(0);

  // Get source buffer (512x240)
  u8 *src = get_ppu_buffer();
  int scrollX = get_scroll_x();

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

  // Draw FPS at center of screen (absolute x=100, so 100 in status bar area)
  int fx = 100;
  int fy = 20; // Y in status bar (rows 0-3)
  u8 color = 0x30; // White
  u32 fps_val = fps > 99999 ? 99999 : fps;

  auto draw_digit = [&](int d) {
    for (int r = 0; r < 5; r++)
      for (int c = 0; c < 3; c++)
        if (digits[d][r] & (4 >> c))
          src[(fy + r) * 512 + fx + c] = color;
    fx += 4;
  };

  if (fps_val >= 10000) draw_digit((fps_val / 10000) % 10);
  if (fps_val >= 1000) draw_digit((fps_val / 1000) % 10);
  if (fps_val >= 100) draw_digit((fps_val / 100) % 10);
  if (fps_val >= 10) draw_digit((fps_val / 10) % 10);
  draw_digit(fps_val % 10);
#endif

  // ===========================================
  // Mode 13h: 320x200 linear framebuffer
  // ===========================================
  // NES output is 256x240. Mode 13h is 320x200.
  // Center horizontally: (320 - 256) / 2 = 32 pixels
  // Crop 20 pixels from top/bottom: show rows 20-219 of the 240-line buffer
  u32 start_x = 32;
  u32 crop_top = 20;
  
  u16 bpl = boot_info->bytes_per_line;
  if (bpl == 0) bpl = 320; // Fallback

  // Status Bar (Rows 0-3, which is y=0 to 31 in buffer)
  // Cropped, so we only see y=20 to 31.
  for (int y = crop_top; y < 32; y++) {
    void *src_row = (void *)&src[y * 512 + 0]; // Always read from x=0
    void *dst_row = (void *)&vram[(y - crop_top) * bpl + start_x];
    u32 dwords = 64; // 256 / 4 = 64 DWORDs
    asm volatile(
        "rep movsl"
        : "+D"(dst_row), "+S"(src_row), "+c"(dwords)
        :
        : "memory"
    );
  }

  // Gameplay Area (Rows 4-29, which is y=32 to 239 in buffer)
  // Cropped at bottom, so we see y=32 to 219.
  for (int y = 32; y < 220; y++) {
    u8 *dst_row = &vram[(y - crop_top) * bpl + start_x];
    
    // Check for wrap-around
    if (scrollX + 256 > 512) {
        int width1 = 512 - scrollX;
        int width2 = 256 - width1;
        
        // Copy end of nametable 1
        void *src1 = &src[y * 512 + scrollX];
        void *dst1 = dst_row;
        u32 dwords1 = width1 / 4;
        asm volatile("rep movsl" : "+D"(dst1), "+S"(src1), "+c"(dwords1) : : "memory");
        
        // Wrap to nametable 0
        void *src2 = &src[y * 512 + 0];
        void *dst2 = dst_row + width1;
        u32 dwords2 = width2 / 4;
        asm volatile("rep movsl" : "+D"(dst2), "+S"(src2), "+c"(dwords2) : : "memory");
    } else {
        void *src_row = &src[y * 512 + scrollX];
        u32 dwords = 64;
        asm volatile(
            "rep movsl"
            : "+D"(dst_row), "+S"(src_row), "+c"(dwords)
            :
            : "memory"
        );
    }
  }

  // Draw sprites directly to VRAM
  draw_sprites_to_vram(vram, start_x, crop_top);
}
} // namespace HAL
