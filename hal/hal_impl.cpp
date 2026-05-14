#include "../kernel/util.hpp"
#include "hal.hpp"
#ifdef GD5428_OPTIMIZE
#include "gd5428.hpp"
#endif
#ifdef SB_SUPPORT
#include "soundblaster.hpp"
#endif
namespace HAL {
void video_init();

void init() {
  serial_init();
  video_init();
#ifdef GD5428_OPTIMIZE
  if (GD5428::init()) {
    // We no longer enable True Linear Packing (SR7 bit 0) because that mode
    // fundamentally breaks Mode 13h pixel doubling timings, turning 320x200 
    // into an invisible 640 physical dot layout (causing double side-by-side screens).
  }
#endif
#ifdef SB_SUPPORT
  HAL::SB::init(SB_BLASTER);
#endif
}
} // namespace HAL

// Simple serial print for TEXT_MODE benchmark
// Outputs to COM1 (0x3F8)
extern "C" void serial_print(const char *str) {
  while (*str) {
    // Wait for transmit buffer empty
    while (!(inb(0x3F8 + 5) & 0x20))
      ;
    outb(0x3F8, *str++);
  }
}
