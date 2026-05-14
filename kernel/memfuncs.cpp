// ============================================================================
// kernel/memfuncs.cpp - Optimized memory functions for freestanding builds
// ============================================================================
// Uses x86 string instructions (rep movsd/stosd) for ~4x speedup over byte
// loops. GCC generates external calls to these at high optimization levels.

#include "types.h"

extern "C" {

// Fast memcpy using rep movsd (32-bit) + rep movsb (remainder)
void *memcpy(void *dst, const void *src, size_t n) {
  void *ret = dst;
  size_t dwords = n / 4;
  size_t bytes = n % 4;

  // Copy 4 bytes at a time
  asm volatile("rep movsl" : "+D"(dst), "+S"(src), "+c"(dwords) : : "memory");

  // Copy remaining bytes
  asm volatile("rep movsb" : "+D"(dst), "+S"(src), "+c"(bytes) : : "memory");

  return ret;
}

// Fast memset using rep stosd (32-bit) + rep stosb (remainder)
void *memset(void *dst, int val, size_t n) {
  void *ret = dst;

  // Replicate byte to all 4 bytes of EAX
  u32 val32 = (u8)val;
  val32 |= val32 << 8;
  val32 |= val32 << 16;

  size_t dwords = n / 4;
  size_t bytes = n % 4;

  // Set 4 bytes at a time
  asm volatile("rep stosl" : "+D"(dst), "+c"(dwords) : "a"(val32) : "memory");

  // Set remaining bytes
  asm volatile("rep stosb" : "+D"(dst), "+c"(bytes) : "a"(val) : "memory");

  return ret;
}

// memmove needs overlap handling, use byte-by-byte for safety
void *memmove(void *dst, const void *src, size_t n) {
  u8 *d = (u8 *)dst;
  const u8 *s = (const u8 *)src;

  if (d < s) {
    // Forward copy is safe
    size_t dwords = n / 4;
    size_t bytes = n % 4;
    asm volatile("rep movsl" : "+D"(d), "+S"(s), "+c"(dwords) : : "memory");
    asm volatile("rep movsb" : "+D"(d), "+S"(s), "+c"(bytes) : : "memory");
  } else if (d > s) {
    // Backward copy needed
    d += n;
    s += n;
    while (n--)
      *--d = *--s;
  }
  return dst;
}

// memcmp - byte comparison (no fast path needed, rarely called)
int memcmp(const void *s1, const void *s2, size_t n) {
  const u8 *p1 = (const u8 *)s1;
  const u8 *p2 = (const u8 *)s2;
  while (n--) {
    if (*p1 != *p2)
      return *p1 - *p2;
    p1++;
    p2++;
  }
  return 0;
}

} // extern "C"
