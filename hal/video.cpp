#include "../kernel/common.hpp"
#include "../smb/SMB.hpp" // For g_smbEngine
#include "hal.hpp"

// ASM tile renderer (Nesticle-style optimization)
extern "C" void draw_tile_fast(const u8 *tile, u8 *dest, const u8 *palette,
                               u8 bg_color);

namespace HAL {

// ========================================================================
// PPU State
// ========================================================================

struct PPUState {
  // Registers
  u8 ctrl;     // $2000 PPUCTRL
  u8 mask;     // $2001 PPUMASK
  u8 status;   // $2002 PPUSTATUS
  u8 oam_addr; // $2003 OAMADDR
  u8 scroll_x; // $2005 PPUSCROLL X
  u8 scroll_y; // $2005 PPUSCROLL Y

  // Internal Latches
  u16 vram_addr;     // Current VRAM address (loopy_v)
  u16 temp_addr;     // Temporary VRAM address (loopy_t)
  u8 fine_x;         // Fine X scroll (3 bits)
  bool write_toggle; // First/Second write toggle (w)
  u8 read_buffer;    // PPUDATA read buffer

  // Memory
  u8 palette[32];      // Palette RAM ($3F00-$3F1F)
  u8 nametables[2048]; // 2KB internal VRAM (Vertical/Horizontal mirroring)
  u8 oam[256];         // Object Attribute Memory (Sprites)

  // Rendering Buffer (320x240 to match Mode 13h/X or windowed)
  // We render to a 256x240 internal buffer, then upscale/copy to VGA
  u32 *framebuffer;
};

static PPUState ppu;

// Default Palette (RGB)
// NES System Palette (64 colors, RGB888 format) - Matches video32.asm
static const u32 palette_rgb[64] = {
    0x7C7C7C, 0x0000FC, 0x0000BC, 0x4428BC, 0x940084, 0xA80020, 0xA81000,
    0x881400, 0x503000, 0x007800, 0x006800, 0x005800, 0x004058, 0x000000,
    0x000000, 0x000000, 0xBCBCBC, 0x0078F8, 0x0058F8, 0x6844FC, 0xD800CC,
    0xE40058, 0xF83800, 0xE45C10, 0xAC7C00, 0x00B800, 0x00A800, 0x00A844,
    0x008888, 0x000000, 0x000000, 0x000000, 0xF8F8F8, 0x3CBCFC, 0x6888FC,
    0x9878F8, 0xF878F8, 0xF85898, 0xF87858, 0xFCA044, 0xF8B800, 0xB8F818,
    0x58D854, 0x58F898, 0x00E8D8, 0x787878, 0x000000, 0x000000, 0xFCFCFC,
    0xA4E4FC, 0xB8B8F8, 0xD8B8F8, 0xF8B8F8, 0xF8A4C0, 0xF0D0B0, 0xFCE0A8,
    0xF8D878, 0xD8F878, 0xB8F8B8, 0xB8F8D8, 0x00FCFC, 0xF8D8F8, 0x000000,
    0x000000};

// ========================================================================
// Internal Helpers
// ========================================================================

static u16 mirror_nametable(u16 addr) {
  // Horizontal mirroring (Vertical arrangement) - SMB uses this?
  // Actually SMB uses Vertical Mirroring (Horizontal arrangement)
  // Wait, SMB-cpp says "mode = 1 // Mirroring mode for Super Mario Bros."
  // And their lookup is {0, 1, 0, 1} for Horizontal.
  // Let's implement standard mirroring logic.
  // For now, simpler: mask to 2KB
  return (addr - 0x2000) % 0x800; // Just fold everything into the 2KB we have
}

static void ppu_increment_addr() {
  if (bit_test(ppu.ctrl, 2)) {
    ppu.vram_addr += 32; // Vertical PPU increment
  } else {
    ppu.vram_addr += 1; // Horizontal PPU increment
  }
}

// ========================================================================
// PPU Memory Access
// ========================================================================

// Forward declaration for dirty tile tracking
static void mark_tile_dirty(u16 addr);

static u8 ppu_mem_read(u16 addr) {
  addr &= 0x3FFF;

  if (addr < 0x2000) {
    // Pattern Tables (CHR ROM)
    // Get CHR data from global engine
    u8 *chr = g_smbEngine ? g_smbEngine->getChrPointer() : nullptr;
    if (chr)
      return chr[addr];
    return 0;
  } else if (addr < 0x3F00) {
    // Nametables
    return ppu.nametables[mirror_nametable(addr)];
  } else if (addr < 0x4000) {
    // Palette
    addr &= 0x1F;
    // Mirroring: $3F10/$3F14/$3F18/$3F1C are mirrors of $3F00/$3F04/$3F08/$3F0C
    if (addr >= 0x10 && (addr & 3) == 0)
      addr -= 0x10;
    return ppu.palette[addr];
  }
  return 0;
}

static void ppu_mem_write(u16 addr, u8 val) {
  addr &= 0x3FFF;

  if (addr < 0x2000) {
    // CHR RAM/ROM - usually read only for SMB
    // SMB uses CHR ROM
  } else if (addr < 0x3F00) {
    // Nametables
    u16 mirrored = mirror_nametable(addr);
    if (ppu.nametables[mirrored] != val) {
      ppu.nametables[mirrored] = val;
      mark_tile_dirty(mirrored); // Track dirty tiles
    }
  } else if (addr < 0x4000) {
    // Palette
    addr &= 0x1F;
    if (addr >= 0x10 && (addr & 3) == 0)
      addr -= 0x10;
    ppu.palette[addr] = val;
  }
}

// ========================================================================
// PPU IO Handlers
// ========================================================================

void ppu_write(u16 addr, u8 val) {
  // Register polling from $2000-$2007 (mirrored every 8 bytes)
  switch (addr & 7) {
  case 0: // $2000 PPUCTRL
    ppu.ctrl = val;
    // t: ...NN.. ........ = d: ...NN...
    ppu.temp_addr = (ppu.temp_addr & 0xF3FF) | ((u16)(val & 0x03) << 10);
    break;

  case 1: // $2001 PPUMASK
    ppu.mask = val;
    break;

  case 3: // $2003 OAMADDR
    ppu.oam_addr = val;
    break;

  case 4: // $2004 OAMDATA
    ppu.oam[ppu.oam_addr++] = val;
    break;

  case 5: // $2005 PPUSCROLL
    if (!ppu.write_toggle) {
      // First write: X scroll
      ppu.scroll_x = val;
      ppu.fine_x = val & 7;
      // t: ....... ...HGFED = d: HGFED...
      ppu.temp_addr = (ppu.temp_addr & 0xFFE0) | (val >> 3);
      ppu.write_toggle = true;
    } else {
      // Second write: Y scroll
      ppu.scroll_y = val;
      // t: CBA..HG FED..... = d: HGFEDCBA
      ppu.temp_addr = (ppu.temp_addr & 0x8FFF) | ((u16)(val & 0x07) << 12);
      ppu.temp_addr = (ppu.temp_addr & 0xFC1F) | ((u16)(val & 0xF8) << 2);
      ppu.write_toggle = false;
    }
    break;

  case 6: // $2006 PPUADDR
    if (!ppu.write_toggle) {
      // First write: High byte
      ppu.temp_addr = (ppu.temp_addr & 0x80FF) | ((u16)(val & 0x3F) << 8);
      ppu.write_toggle = true;
    } else {
      // Second write: Low byte
      ppu.temp_addr = (ppu.temp_addr & 0xFF00) | val;
      ppu.vram_addr = ppu.temp_addr;
      ppu.write_toggle = false;
    }
    break;

  case 7: // $2007 PPUDATA
    ppu_mem_write(ppu.vram_addr, val);
    ppu_increment_addr();
    break;
  }
}

u8 ppu_read(u16 addr) {
  static int status_cycle = 0; // Alternating cycle for sprite0/vblank hack
  u8 val = 0;
  switch (addr & 7) {
  case 2: // $2002 PPUSTATUS
    // HACK: Alternate between VBlank+Sprite0 set and cleared
    // This satisfies both Sprite0Clr (poll until clear) and Sprite0Hit (poll
    // until set) loops Reference: SMB-cpp returns (cycle++ % 2 == 0 ? 0xc0 : 0)
    val = (status_cycle++ % 2 == 0) ? 0xC0 : 0x00;
    ppu.write_toggle = false;
    break;

  case 4: // $2004 OAMDATA
    val = ppu.oam[ppu.oam_addr];
    break;

  case 7: // $2007 PPUDATA
    // Buffered read logic
    val = ppu.read_buffer;
    ppu.read_buffer = ppu_mem_read(ppu.vram_addr);

    // Palette reads are not buffered
    if (ppu.vram_addr >= 0x3F00) {
      val = ppu.read_buffer;
    }

    ppu_increment_addr();
    break;
  }
  return val;
}

void ppu_set_vblank() {
  bit_set(&ppu.status, 7);
  // Also Trigger NMI if enabled (PPUCTRL bit 7)
  // For now, we rely on the game polling $2002
}

void ppu_oam_dma(u8 page) {
  // Need to access CPU RAM, which we don't have a clean interface for yet.
  // Assuming SMB::ram matches first 2KB, but OAM DMA can come from ROM too?
  // For SMB, it's usually from RAM ($0200-$0700).
  // Let's implement a stub that assumes RAM access.
  u16 start_addr = (u16)page << 8;

  // Safety check: ensure we are reading from valid RAM
  if (start_addr < 0x800 || (start_addr >= 0x6000 && start_addr < 0x8000)) {
    // Standard RAM or WRAM
    // Read from the engine's RAM via accessor
    if (!g_smbEngine)
      return;

    // Fast copy
    for (int i = 0; i < 256; i++) {
      ppu.oam[i] = g_smbEngine->readRam(start_addr + i);
    }
  } else {
    // DMA from ROM?
    // For now handle the common case: SMB usually DMAs from $0200 (RAM)
    if (!g_smbEngine)
      return;
    for (int i = 0; i < 256; i++) {
      if (start_addr == 0x0200) {
        ppu.oam[i] = g_smbEngine->readRam(start_addr + i);
      }
    }
  }
}

// ========================================================================
// Rendering Logic
// ========================================================================

// Pre-decoded tile cache (Nesticle-style optimization)
// NES tiles are 2-bitplane format: 16 bytes -> 8x8 pixels
// Pre-decode to 64 bytes per tile (1 byte per pixel, values 0-3)
// 512 tiles total (256 in each pattern table)
static u8 tile_cache[512][64]; // [tile_id][row*8 + col] = color index 0-3
static bool tile_cache_valid = false;

// Decode all tiles from CHR data into cache
static void decode_all_tiles(const u8 *chr) {
  for (int tile = 0; tile < 512; tile++) {
    const u8 *pattern = &chr[tile * 16];
    u8 *decoded = tile_cache[tile];

    for (int row = 0; row < 8; row++) {
      u8 low = pattern[row];
      u8 high = pattern[row + 8];

      // Unrolled: decode 8 pixels per row
      decoded[row * 8 + 0] = ((high >> 7) & 1) << 1 | ((low >> 7) & 1);
      decoded[row * 8 + 1] = ((high >> 6) & 1) << 1 | ((low >> 6) & 1);
      decoded[row * 8 + 2] = ((high >> 5) & 1) << 1 | ((low >> 5) & 1);
      decoded[row * 8 + 3] = ((high >> 4) & 1) << 1 | ((low >> 4) & 1);
      decoded[row * 8 + 4] = ((high >> 3) & 1) << 1 | ((low >> 3) & 1);
      decoded[row * 8 + 5] = ((high >> 2) & 1) << 1 | ((low >> 2) & 1);
      decoded[row * 8 + 6] = ((high >> 1) & 1) << 1 | ((low >> 1) & 1);
      decoded[row * 8 + 7] = ((high >> 0) & 1) << 1 | ((low >> 0) & 1);
    }
  }
  tile_cache_valid = true;
}

// ========================================================================
// Dirty Tile Tracking (Nesticle-style optimization)
// ========================================================================
// Track which tiles have changed since last frame
// NES has 2 nametables of 32x30 tiles each
static u8 dirty_tiles[2][32 * 30];  // 0 = clean, 1 = dirty
static bool all_tiles_dirty = true; // Force full redraw on first frame
static u8 last_scroll_x = 0;        // Track scroll changes

// Mark a tile as dirty (called from ppu_mem_write)
static void mark_tile_dirty(u16 addr) {
  // addr is already mirrored, should be 0x000-0x7FF range
  if (addr < 0x3C0) { // Nametable bytes (not attributes)
    int nt = (addr >= 0x400) ? 1 : 0;
    int tile_idx = addr & 0x3FF;
    if (tile_idx < 32 * 30) {
      dirty_tiles[nt][tile_idx] = 1;
    }
  } else {
    // Attribute table changed - mark 16 tiles dirty (4x4 area)
    int nt = (addr >= 0x400) ? 1 : 0;
    int attr_idx = (addr & 0x3F);
    int base_x = (attr_idx % 8) * 4;
    int base_y = (attr_idx / 8) * 4;

    for (int dy = 0; dy < 4 && base_y + dy < 30; dy++) {
      for (int dx = 0; dx < 4; dx++) {
        int tile_idx = (base_y + dy) * 32 + base_x + dx;
        if (tile_idx < 32 * 30) {
          dirty_tiles[nt][tile_idx] = 1;
        }
      }
    }
  }
}

// Mark all tiles dirty (for scroll changes or first frame)
static void mark_all_dirty() {
  memset(dirty_tiles, 1, sizeof(dirty_tiles));
  all_tiles_dirty = true;
}

// Clear dirty flags after rendering
static void clear_dirty_flags() {
  memset(dirty_tiles, 0, sizeof(dirty_tiles));
  all_tiles_dirty = false;
}

// Internal buffer for PPU render
static u8 internal_buffer[256 * 240];

u8 *get_ppu_buffer() { return internal_buffer; }

// Initialize VGA DAC with NES palette
static void setup_vga_palette() {
  // Port 0x3C8: Palette Index Write
  // Port 0x3C9: Palette Data Write (R, G, B) - 3 writes per index
  // VGA is 6-bit per channel (0-63)

  outb(0x3C8, 0); // Start at index 0

  for (int i = 0; i < 64; i++) {
    u32 rgb = palette_rgb[i];
    u8 r = (rgb >> 16) & 0xFF;
    u8 g = (rgb >> 8) & 0xFF;
    u8 b = (rgb >> 0) & 0xFF;

    // Shift down to 6-bit
    outb(0x3C9, r >> 2);
    outb(0x3C9, g >> 2);
    outb(0x3C9, b >> 2);
  }
}

void ppu_render_frame(u32 *buffer_unused) {

  // FORCE ENABLE RENDERING (DEBUG)
  if ((ppu.mask & 0x18) == 0) {
    ppu.mask |= 0x1E; // Enable BG/Spr + NoClip
  }

  // Ignored buffer_unused, we render to internal_buffer
  // because type mismatch (u32 vs u8) in previous definition.
  // We will fix header later or just cast.
  // Actually, let's just write to internal_buffer.
  u8 *dst = internal_buffer;

  u8 *chr = g_smbEngine ? g_smbEngine->getChrPointer() : nullptr;
  if (!chr)
    return;

  // PPU Control: Bit 4 = Bg Pattern Table (0: $0000, 1: $1000)
  u16 bg_pattern_base = (ppu.ctrl & 0x10) ? 0x1000 : 0x0000;

  // Calculate scroll X including nametable select bit
  int scrollX = (int)ppu.scroll_x + ((ppu.ctrl & 0x01) ? 256 : 0);

  // Ensure tile cache is decoded
  if (!tile_cache_valid) {
    decode_all_tiles(chr);
  }

  // Fast tile renderer using pre-decoded cache
  // screen_x/screen_y_base are pixel coordinates
  auto renderTileAt = [&](u16 nt_addr, int screen_x, int screen_y_base) {
    u8 tile_id = ppu.nametables[mirror_nametable(nt_addr)];

    // Calculate attribute table address
    int tile_x = (nt_addr & 0x1F);
    int tile_y = ((nt_addr >> 5) & 0x1F);
    u16 attr_base = (nt_addr & 0x0C00) + 0x23C0;
    u16 attr_addr = attr_base + (tile_y / 4) * 8 + (tile_x / 4);
    u8 attr_byte = ppu.nametables[mirror_nametable(attr_addr)];

    int shift = ((tile_y & 2) << 1) | (tile_x & 2);
    u8 palette_high = (attr_byte >> shift) & 3;

    // Use cached tile (already decoded!)
    int pattern_idx = (bg_pattern_base / 16) + tile_id;
    const u8 *decoded = tile_cache[pattern_idx];

    // Precompute palette base address
    u8 bg_color = ppu.palette[0] & 0x3F;
    const u8 *pal_base = &ppu.palette[4 * palette_high];

    // Fast path: tile fully on-screen (no clipping needed)
    if (screen_x >= 0 && screen_x <= 248 && screen_y_base >= 0 &&
        screen_y_base <= 232) {
      // Use ASM tile renderer for maximum speed
      u8 *dest_ptr = &dst[screen_y_base * 256 + screen_x];
      draw_tile_fast(decoded, dest_ptr, pal_base, bg_color);
    } else {
      // Slow path: clipped tile
      for (int py = 0; py < 8; py++) {
        int screen_y = screen_y_base + py;
        if (screen_y < 0 || screen_y >= 240)
          continue;

        for (int px = 0; px < 8; px++) {
          int sx = screen_x + px;
          if (sx < 0 || sx >= 256)
            continue;

          u8 color_idx = decoded[py * 8 + px];
          dst[screen_y * 256 + sx] =
              color_idx ? (pal_base[color_idx] & 0x3F) : bg_color;
        }
      }
    }
  };

  // ============================================
  // Detect scroll changes - force full redraw if scroll moved
  // ============================================
  u8 current_scroll = (u8)scrollX;
  if (current_scroll != last_scroll_x) {
    last_scroll_x = current_scroll;
    // Scroll changed - mark all tiles dirty for full redraw
    memset(dirty_tiles, 1, sizeof(dirty_tiles));
    all_tiles_dirty = true;
  }

  // Pre-fill buffer with background color on full redraw
  u8 bg_color_fill = ppu.palette[0] & 0x3F;
  if (all_tiles_dirty) {
    memset(dst, bg_color_fill, 256 * 240);
  }

  // ============================================
  // Render Status Bar (rows 0-3) - NO SCROLL
  // ============================================
  for (int tile_y = 0; tile_y < 4; tile_y++) {
    for (int tile_x = 0; tile_x < 32; tile_x++) {
      // Skip clean tiles
      int idx = tile_y * 32 + tile_x;
      if (!all_tiles_dirty && !dirty_tiles[0][idx]) continue;

      u16 nt_addr = 0x2000 + tile_y * 32 + tile_x;
      renderTileAt(nt_addr, tile_x * 8, tile_y * 8);
    }
  }

  // ============================================
  // Render Gameplay Area (rows 4-29) - WITH SCROLL
  // ============================================
  int xMin = scrollX / 8;
  int xMax = (scrollX + 255) / 8;

  for (int x = xMin; x <= xMax; x++) {
    for (int tile_y = 4; tile_y < 30; tile_y++) {
      // Skip clean tiles
      int nt = (x < 32) ? 0 : 1;
      int local_x = x & 0x1F;
      int idx = tile_y * 32 + local_x;
      if (!all_tiles_dirty && !dirty_tiles[nt][idx]) continue;

      // Determine nametable based on x position
      u16 nt_addr;
      if (x < 32) {
        nt_addr = 0x2000 + tile_y * 32 + x;
      } else if (x < 64) {
        nt_addr = 0x2400 + tile_y * 32 + (x - 32);
      } else {
        nt_addr = 0x2000 + tile_y * 32 + (x - 64); // Wrap around
      }

      // Calculate screen position with scroll offset
      int screen_x = (x * 8) - scrollX;
      renderTileAt(nt_addr, screen_x, tile_y * 8);
    }
  }

  // ============================================
  // Render Sprites BEHIND Background (priority bit = 1)
  // ============================================
  // Lambda to render sprites with optional priority filter
  auto renderSprites = [&](bool behind_bg) {
    if (!(ppu.mask & 0x10))
      return; // Sprites disabled

    u16 spr_pattern_base = (ppu.ctrl & 0x08) ? 0x1000 : 0x0000;

    for (int i = 63; i >= 0; i--) {
      // Skip inactive sprites early (before reading other OAM bytes)
      u8 spr_y = ppu.oam[i * 4 + 0];
      if (spr_y >= 0xEF) continue;

      u8 tile_id = ppu.oam[i * 4 + 1];
      u8 attr = ppu.oam[i * 4 + 2];
      u8 spr_x = ppu.oam[i * 4 + 3];

      // Check priority bit - skip if not matching current pass
      bool has_priority = (attr & 0x20) != 0; // Bit 5 = behind BG
      if (has_priority != behind_bg)
        continue;

      // Skip sprites off-screen horizontally
      if (spr_x >= 0xF9)
        continue;

      // Sprite Y is 1 scanline early
      spr_y++;

      bool flip_x = (attr & 0x40) != 0;
      bool flip_y = (attr & 0x80) != 0;
      u8 palette_idx = (attr & 0x03);

      // Use pre-decoded tile cache (FAST!)
      int pattern_idx = (spr_pattern_base / 16) + tile_id;
      const u8 *decoded = tile_cache[pattern_idx];
      const u8 *spr_pal = &ppu.palette[0x10 + palette_idx * 4];
      u8 bg_color = ppu.palette[0] & 0x3F;

      // Fast path: fully on-screen sprite (no clipping)
      if (spr_x <= 248 && spr_y >= 1 && spr_y <= 232) {
        for (int row = 0; row < 8; row++) {
          int yOffset = flip_y ? (7 - row) : row;
          int screen_y = spr_y + yOffset;
          u8 *dst_row = &dst[screen_y * 256 + spr_x];
          const u8 *src_row = &decoded[row * 8];

          for (int col = 0; col < 8; col++) {
            int xOffset = flip_x ? (7 - col) : col;
            u8 color_idx = src_row[col];

            if (color_idx == 0)
              continue; // Transparent

            // For behind-BG sprites: only draw if BG pixel is bg_color
            if (behind_bg && dst_row[xOffset] != bg_color)
              continue;

            dst_row[xOffset] = spr_pal[color_idx] & 0x3F;
          }
        }
      } else {
        // Slow path: clipped sprite
        for (int row = 0; row < 8; row++) {
          int yOffset = flip_y ? (7 - row) : row;
          int screen_y = spr_y + yOffset;
          if (screen_y < 0 || screen_y >= 240)
            continue;

          const u8 *src_row = &decoded[row * 8];

          for (int col = 0; col < 8; col++) {
            int xOffset = flip_x ? (7 - col) : col;
            int screen_x = spr_x + xOffset;

            if (screen_x < 0 || screen_x >= 256)
              continue;

            u8 color_idx = src_row[col];
            if (color_idx == 0)
              continue;

            int pixel_idx = screen_y * 256 + screen_x;
            if (behind_bg && dst[pixel_idx] != bg_color)
              continue;

            dst[pixel_idx] = spr_pal[color_idx] & 0x3F;
          }
        }
      }
    }
  };

  // First: render behind-BG sprites (they appear behind non-transparent
  // BG) These only draw where the background pixel is the universal BG
  // color (palette[0])
  renderSprites(true);

  // Then: render in-front-of-BG sprites (priority bit = 0)
  renderSprites(false);

  // Clear dirty flags for next frame
  clear_dirty_flags();
}

void video_init() {
  ppu.write_toggle = false;
  ppu.vram_addr = 0;
  ppu.temp_addr = 0;

  setup_vga_palette();
}
} // namespace HAL
