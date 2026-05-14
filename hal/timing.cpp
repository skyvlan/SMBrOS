#include "../kernel/util.hpp"
#include "hal.hpp"

namespace HAL {

// ========================================================================
// Timing Implementation - IRQ-based 60.0988 Hz
// ========================================================================
// Uses PIT IRQ0 to signal frame ready. CPU halts between frames.

// External: Set by IRQ0 handler in kernel/idt.cpp
extern "C" volatile u32 g_frame_ready;
extern "C" volatile u32 g_tick_count;

// Initialize IDT (called once from kernel_main)
extern "C" void idt_init();

static bool timing_initialized = false;

// Initialize timing subsystem (call this before using g_tick_count)
void init_timing() {
  if (!timing_initialized) {
    idt_init();
    timing_initialized = true;
  }
}

void wait_vsync() {
  // Initialize IDT/PIC/PIT on first call
  if (!timing_initialized) {
    init_timing();
  }

  // Clear the frame ready flag
  g_frame_ready = 0;

  // Wait for IRQ0 to set the flag
  // Use HLT to save CPU cycles - wakes on any interrupt
  while (!g_frame_ready) {
    asm volatile("hlt");
  }
}

} // namespace HAL
