#include "soundblaster.hpp"
#include "../kernel/util.hpp"

namespace HAL {
namespace SB {

static SBConfig g_config;
static bool g_initialized = false;

// Basic parser for "A220 I7 D1 H1 P330 T6"
// Assuming rigid order and spacing as requested by the user.
static void parse_blaster_env(const char* env) {
    // Default values
    g_config.base_port = 0x220;
    g_config.irq = 7;
    g_config.dma8 = 1;
    g_config.dma16 = 5;
    g_config.mpu_port = 0x330;
    g_config.type = 6;

    if (!env) return;

    // A simple custom parser mapping hex to ports and dec to IRQ/DMA
    const char* ptr = env;
    while (*ptr) {
        if (*ptr == 'A') {
            ptr++;
            g_config.base_port = 0;
            while ((*ptr >= '0' && *ptr <= '9') || (*ptr >= 'A' && *ptr <= 'F') || (*ptr >= 'a' && *ptr <= 'f')) {
                u16 digit = 0;
                if (*ptr >= '0' && *ptr <= '9') digit = *ptr - '0';
                else if (*ptr >= 'A' && *ptr <= 'F') digit = *ptr - 'A' + 10;
                else if (*ptr >= 'a' && *ptr <= 'f') digit = *ptr - 'a' + 10;
                g_config.base_port = (g_config.base_port << 4) | digit;
                ptr++;
            }
        } else if (*ptr == 'I') {
            ptr++;
            g_config.irq = 0;
            while (*ptr >= '0' && *ptr <= '9') {
                g_config.irq = (g_config.irq * 10) + (*ptr - '0');
                ptr++;
            }
        } else if (*ptr == 'D') {
            ptr++;
            g_config.dma8 = 0;
            while (*ptr >= '0' && *ptr <= '9') {
                g_config.dma8 = (g_config.dma8 * 10) + (*ptr - '0');
                ptr++;
            }
        } else if (*ptr == 'H') {
            ptr++;
            g_config.dma16 = 0;
            while (*ptr >= '0' && *ptr <= '9') {
                g_config.dma16 = (g_config.dma16 * 10) + (*ptr - '0');
                ptr++;
            }
        } else if (*ptr == 'P') {
            ptr++;
            g_config.mpu_port = 0;
            while ((*ptr >= '0' && *ptr <= '9') || (*ptr >= 'A' && *ptr <= 'F') || (*ptr >= 'a' && *ptr <= 'f')) {
                u16 digit = 0;
                if (*ptr >= '0' && *ptr <= '9') digit = *ptr - '0';
                else if (*ptr >= 'A' && *ptr <= 'F') digit = *ptr - 'A' + 10;
                else if (*ptr >= 'a' && *ptr <= 'f') digit = *ptr - 'a' + 10;
                g_config.mpu_port = (g_config.mpu_port << 4) | digit;
                ptr++;
            }
        } else if (*ptr == 'T') {
            ptr++;
            g_config.type = 0;
            while (*ptr >= '0' && *ptr <= '9') {
                g_config.type = (g_config.type * 10) + (*ptr - '0');
                ptr++;
            }
        } else {
            ptr++; // Skip spaces or unknown chars
        }
    }
}

// Reset the DSP
static bool reset_dsp() {
    u16 reset_port = g_config.base_port + 0x6;
    u16 read_port  = g_config.base_port + 0xA;

    outb(reset_port, 1);
    
    // Delay (at least 3 microseconds)
    for (volatile int i = 0; i < 1000; i++) {}

    outb(reset_port, 0);

    // Wait for ready (max 100us)
    for (int i = 0; i < 1000; i++) {
        u8 status = inb(g_config.base_port + 0xE);
        if (status & 0x80) { // Data available
            if (inb(read_port) == 0xAA) {
                return true;
            }
        }
    }
    return false;
}

#ifdef SB_MODE_MIDI_SYNTH
// OPL3 initialization via direct Adlib ports
static void opl3_wait_index() {
    for (volatile int i = 0; i < 6; i++) {
        inb(g_config.base_port); // or 0x388
    }
}

static void opl3_wait_data() {
    for (volatile int i = 0; i < 35; i++) {
        inb(g_config.base_port); // or 0x388
    }
}

static void opl3_write(u8 reg, u8 data) {
    outb(0x388, reg);
    opl3_wait_index();
    outb(0x389, data);
    opl3_wait_data();
}

static void init_opl3() {
    // Clear all registers
    for (int i = 0; i <= 0xF5; i++) {
        opl3_write(i, 0);
    }
    // Enable OPL3 extensions (Port 0x38A, 0x38B sequence for Adlib / SB)
    // Here we're using generic port 0x388. Alternatively we could use g_config.base_port.
    // For now we assume standard ports.
}
#endif

bool init(const char* blaster_env) {
    parse_blaster_env(blaster_env);

    if (!reset_dsp()) {
        return false; // DSP failed to reset
    }

#ifdef SB_MODE_MIDI_SYNTH
    init_opl3();
#elif defined(SB_MODE_NES)
    // Setup DMA PCM buffer stuff here 
    // (Stub for NES DMA)
#endif

    g_initialized = true;
    return true;
}

void write_apu(u16 addr, u8 val) {
    if (!g_initialized) return;

#ifdef SB_MODE_MIDI_SYNTH
    // Map NES APU to OPL3 (Stub)
    (void)addr;
    (void)val;
#elif defined(SB_MODE_NES)
    // Map NES APU to PCM buffers (Stub)
    (void)addr;
    (void)val;
#endif
}

} // namespace SB
} // namespace HAL
