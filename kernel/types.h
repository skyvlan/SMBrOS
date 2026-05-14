#ifndef TYPES_H
#define TYPES_H

/**
 * Fixed-width integer types for freestanding C/C++
 * No dependency on <stdint.h> or <cstdint>
 * Based on BareMetalNes types.h
 */

// Unsigned integers
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

// Signed integers
typedef signed char        i8;
typedef signed short       i16;
typedef signed int         i32;
typedef signed long long   i64;

// Size types
typedef u32 size_t;
typedef u32 uintptr_t;

// Boolean (C++ has bool, but we need it for C compatibility)
#ifndef __cplusplus
typedef u8 bool;
#define true  (1)
#define false (0)
#endif

#ifndef NULL
#define NULL (0)
#endif

// NES-specific aliases for readability
typedef u8  byte;
typedef u16 word;
typedef u32 dword;

// Fixed-width type aliases matching <stdint.h> names
typedef u8  uint8_t;
typedef u16 uint16_t;
typedef u32 uint32_t;
typedef u64 uint64_t;
typedef i8  int8_t;
typedef i16 int16_t;
typedef i32 int32_t;
typedef i64 int64_t;

#endif // TYPES_H
