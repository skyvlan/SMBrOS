#ifndef GD5428_HPP
#define GD5428_HPP

#ifdef GD5428_OPTIMIZE

#include "../kernel/types.h"

namespace GD5428 {

// Initialize and detect GD5428. Returns true if detected.
bool init();

// Runtime check — safe to call without init
bool is_available();

// Reconfigure Mode 13h to True Linear (discards Chain-4 structure)
void enable_linear_packed_mode();

// Wait for BitBLT engine to become idle
void wait_idle();

// Host-to-screen BitBLT setup
// After calling, write (width * height) bytes to VGA aperture (0xA0000)
// BLT engine handles dest pitch (row padding)
void setup_host_blt(u32 dest_offset, u16 width, u16 height, u16 dest_pitch);

// Screen-to-screen copy (VRAM → VRAM)
// Handles overlapping regions with correct direction
void screen_copy(u32 src_offset, u32 dest_offset,
                 u16 width, u16 height, u16 pitch);

} // namespace GD5428

#endif // GD5428_OPTIMIZE
#endif // GD5428_HPP
