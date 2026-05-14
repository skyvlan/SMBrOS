#ifndef HAL_HPP
#define HAL_HPP

#include "../kernel/types.h"
#include "../kernel/util.hpp"

/**
 * Hardware Abstraction Layer (HAL) Interface
 * Bridges the transpiled game code (SMB namespace) to the hardware (or
 * emulation).
 */

namespace HAL {
// ========================================================================
// System Lifecycle
// ========================================================================

/**
 * Initialize HAL subsystems (Video, Input, Timing, Audio).
 * Must be called before any other HAL function.
 */
void init();

/**
 * Wait for the next Vertical Blank interval.
 * This synchronizes the game loop to 60Hz.
 */
void wait_vsync();

/**
 * Render the current frame to the screen.
 * Copies the PPU internal buffer to the VGA framebuffer.
 */
void render();

// ========================================================================
// PPU (Pixel Processing Unit) Access
// Called by transpiled 6502 code to access memory-mapped PPU registers.
// ========================================================================

/**
 * Initialize PPU.
 */
void ppu_init();

/**
 * Write to a PPU register ($2000-$2007).
 * @param addr Absolute address (e.g., 0x2000)
 * @param val Value to write
 */
void ppu_write(u16 addr, u8 val);

/**
 * Read from a PPU register ($2000-$2007).
 * @param addr Absolute address (e.g., 0x2002)
 * @return Value read
 */
u8 ppu_read(u16 addr);

/**
 * Direct Sprite DMA transfer ($4014).
 * Copies 256 bytes from CPU RAM page to OAM.
 * @param page Page number (e.g., 0x02 for $0200-$02FF)
 */
void ppu_oam_dma(u8 page);

/**
 * Render the full PPU frame to the internal buffer.
 * @param buffer Ignored (internal buffer used)
 */
void ppu_render_frame(u32 *buffer_unused);

/**
 * Set VBlank flag in PPUSTATUS.
 */
void ppu_set_vblank();

// Get pointer to PPU internal buffer (256x240x8 index)
u8 *get_ppu_buffer();

// ========================================================================
// Input Access
// ========================================================================

/**
 * Read from controller ports ($4016/$4017).
 * @param port 0 for Controller 1 ($4016), 1 for Controller 2 ($4017)
 * @return Serialized button state (1 bit per read)
 */
u8 input_read(u8 port);

/**
 * Write to input strobe ($4016).
 * Writing 1 then 0 latches the controller state.
 * @param val Value to write
 */
void input_write(u8 val);

// ========================================================================
// APU (Audio Processing Unit) Access
// ========================================================================

/**
 * Write to an APU register ($4000-$4017).
 */
void apu_write(u16 addr, u8 val);

/**
 * Read from an APU status ($4015).
 */
u8 apu_read(u16 addr);
} // namespace HAL

#endif // HAL_HPP
