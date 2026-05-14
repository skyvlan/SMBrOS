#ifndef COMMON_HPP
#define COMMON_HPP

#include "types.h"

/**
 * Common bit manipulation utilities
 * Based on BareMetalNes common.h
 */

// ============================================================================
// Bit Testing
// ============================================================================

static inline bool bit_test(u32 val, u8 bit) {
    return (val >> bit) & 1;
}

static inline bool bit_test8(u8 val, u8 bit) {
    return (val >> bit) & 1;
}

// ============================================================================
// Bit Manipulation (Byte)
// ============================================================================

static inline void bit_set(u8* val, u8 bit) {
    *val |= (1 << bit);
}

static inline void bit_clear(u8* val, u8 bit) {
    *val &= ~(1 << bit);
}

static inline void bit_toggle(u8* val, u8 bit) {
    *val ^= (1 << bit);
}

static inline void bit_write(u8* val, u8 bit, bool set) {
    if (set) bit_set(val, bit);
    else bit_clear(val, bit);
}

// ============================================================================
// Bit Manipulation (Word)
// ============================================================================

static inline void bit_set16(u16* val, u8 bit) {
    *val |= (1 << bit);
}

static inline void bit_clear16(u16* val, u8 bit) {
    *val &= ~(1 << bit);
}

// ============================================================================
// Min/Max/Clamp
// ============================================================================

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define CLAMP(x, lo, hi) (MAX(lo, MIN(x, hi)))

// ============================================================================
// Alignment
// ============================================================================

#define ALIGN_UP(x, align)   (((x) + ((align) - 1)) & ~((align) - 1))
#define ALIGN_DOWN(x, align) ((x) & ~((align) - 1))

// ============================================================================
// Array Size
// ============================================================================

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#endif // COMMON_HPP
