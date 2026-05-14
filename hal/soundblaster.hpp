#ifndef SOUNDBLASTER_HPP
#define SOUNDBLASTER_HPP

#include "../kernel/types.h"

namespace HAL {
namespace SB {

// The parsed SoundBlaster environment variables
struct SBConfig {
    u16 base_port;  // A (e.g. 0x220)
    u8  irq;        // I (e.g. 7)
    u8  dma8;       // D (e.g. 1)
    u8  dma16;      // H (e.g. 1 or 5)
    u16 mpu_port;   // P (e.g. 0x330)
    u8  type;       // T (e.g. 6)
};

/**
 * Initialize Sound Blaster support.
 * Parses the SB_BLASTER environment string and initializes hardware
 * based on the compiled SB_MODE flag.
 * @param blaster_env The BLASTER string (e.g., "A220 I7 D1 H1 P330 T6")
 * @return True if initialized successfully, false otherwise.
 */
bool init(const char* blaster_env);

/**
 * Handle writes to the NES APU, translating them to Sound Blaster output
 * (either OPL3 or PCM depending on configuration).
 */
void write_apu(u16 addr, u8 val);

} // namespace SB
} // namespace HAL

#endif // SOUNDBLASTER_HPP
