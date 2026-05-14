#ifndef UTIL_HPP
#define UTIL_HPP

#include "types.h"

/**
 * Utility functions for bare-metal x86
 * Port I/O, memory operations, interrupt control
 */

// ============================================================================
// Port I/O (inline ASM)
// ============================================================================

static inline u8 inb(u16 port) {
    u8 result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "dN"(port));
    return result;
}

static inline void outb(u16 port, u8 data) {
    __asm__ volatile("outb %1, %0" : : "dN"(port), "a"(data));
}

static inline u16 inw(u16 port) {
    u16 result;
    __asm__ volatile("inw %1, %0" : "=a"(result) : "dN"(port));
    return result;
}

static inline void outw(u16 port, u16 data) {
    __asm__ volatile("outw %1, %0" : : "dN"(port), "a"(data));
}

static inline u32 inl(u16 port) {
    u32 result;
    __asm__ volatile("inl %1, %0" : "=a"(result) : "dN"(port));
    return result;
}

static inline void outl(u16 port, u32 data) {
    __asm__ volatile("outl %1, %0" : : "dN"(port), "a"(data));
}

// I/O wait (delay for slow devices)
static inline void io_wait() {
    outb(0x80, 0);
}

// ============================================================================
// Memory Operations
// ============================================================================

extern "C" {
    void* memset(void* dst, int val, size_t n);
    void* memcpy(void* dst, const void* src, size_t n);
    int memcmp(const void* s1, const void* s2, size_t n);
    void* memmove(void* dst, const void* src, size_t n);
}

// ============================================================================
// String Operations
// ============================================================================

static inline size_t strlen(const char* s) {
    size_t len = 0;
    while (*s++) len++;
    return len;
}

static inline char* strcpy(char* dst, const char* src) {
    char* d = dst;
    while ((*d++ = *src++));
    return dst;
}

// ============================================================================
// Interrupt Control
// ============================================================================

#define CLI() __asm__ volatile("cli")
#define STI() __asm__ volatile("sti")
#define HLT() __asm__ volatile("hlt")
#define NOP() __asm__ volatile("nop")

static inline void halt_loop() {
    while (true) {
        __asm__ volatile("cli; hlt");
    }
}

// ============================================================================
// Serial Port Debugging (COM1 = 0x3F8)
// ============================================================================

#define COM1 0x3F8

static inline void serial_init() {
    outb(COM1 + 1, 0x00);  // Disable interrupts
    outb(COM1 + 3, 0x80);  // Enable DLAB
    outb(COM1 + 0, 0x03);  // Baud rate divisor lo (38400)
    outb(COM1 + 1, 0x00);  // Baud rate divisor hi
    outb(COM1 + 3, 0x03);  // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xC7);  // Enable FIFO
    outb(COM1 + 4, 0x0B);  // IRQs enabled, RTS/DSR set
}

static inline bool serial_is_transmit_empty() {
    return inb(COM1 + 5) & 0x20;
}

static inline void serial_putchar(char c) {
    while (!serial_is_transmit_empty());
    outb(COM1, c);
}

static inline void serial_puts(const char* s) {
    while (*s) {
        if (*s == '\n') serial_putchar('\r');
        serial_putchar(*s++);
    }
}

static inline void serial_puthex(u32 value, int digits) {
    const char* hex = "0123456789ABCDEF";
    for (int i = digits - 1; i >= 0; i--) {
        serial_putchar(hex[(value >> (i * 4)) & 0xF]);
    }
}

#endif // UTIL_HPP
