// ============================================================================
// kernel/idt.cpp - Interrupt Descriptor Table and PIT IRQ Handler
// ============================================================================
#include "../kernel/util.hpp"

extern "C" {

// IDT Entry structure
struct IDTEntry {
  u16 offset_low;
  u16 selector;
  u8 zero;
  u8 type_attr;
  u16 offset_high;
} __attribute__((packed));

// IDT Pointer
struct IDTPointer {
  u16 limit;
  u32 base;
} __attribute__((packed));

// IDT with 256 entries
static IDTEntry idt_entries[256];
static IDTPointer idt_ptr;

// Frame ready flag - set by IRQ0 handler
volatile u32 g_frame_ready = 0;
volatile u32 g_tick_count = 0;

// Set an IDT entry
static void idt_set_gate(u8 num, u32 base, u16 selector, u8 flags) {
  idt_entries[num].offset_low = base & 0xFFFF;
  idt_entries[num].offset_high = (base >> 16) & 0xFFFF;
  idt_entries[num].selector = selector;
  idt_entries[num].zero = 0;
  idt_entries[num].type_attr = flags;
}

// PIC Ports
#define PIC1_CMD 0x20
#define PIC1_DATA 0x21
#define PIC2_CMD 0xA0
#define PIC2_DATA 0xA1

// Remap PIC to avoid conflicts with CPU exceptions (IRQ0->32, IRQ8->40)
static void pic_remap() {
  // ICW1: Initialize + ICW4 needed
  outb(PIC1_CMD, 0x11);
  outb(PIC2_CMD, 0x11);

  // ICW2: Vector offsets
  outb(PIC1_DATA, 0x20); // IRQ 0-7 -> INT 32-39
  outb(PIC2_DATA, 0x28); // IRQ 8-15 -> INT 40-47

  // ICW3: Cascade
  outb(PIC1_DATA, 0x04); // Slave at IRQ2
  outb(PIC2_DATA, 0x02); // Slave ID 2

  // ICW4: 8086 mode
  outb(PIC1_DATA, 0x01);
  outb(PIC2_DATA, 0x01);

  // Mask all interrupts except IRQ0 (PIT)
  // Note: IRQ1 (keyboard) is handled via polling in input.cpp
  outb(PIC1_DATA, 0xFE); // Only IRQ0 enabled
  outb(PIC2_DATA, 0xFF); // All slave IRQs disabled
}

// IRQ0 (PIT Timer) Handler - called from ASM stub
void irq0_handler() {
  g_tick_count++;
  g_frame_ready = 1;

  // Send EOI to PIC
  outb(PIC1_CMD, 0x20);
}

// PIT Initialization
#define PIT_CH0_DATA 0x40
#define PIT_COMMAND 0x43
#define NES_FRAME_DIVISOR 19912 // ~60.0988 Hz

static void pit_init() {
  // Channel 0, Access Mode LSB/MSB, Mode 2 (Rate Generator), Binary
  outb(PIT_COMMAND, 0x34);

  // Set divisor for ~60Hz
  outb(PIT_CH0_DATA, NES_FRAME_DIVISOR & 0xFF);
  outb(PIT_CH0_DATA, (NES_FRAME_DIVISOR >> 8) & 0xFF);
}

// ASM IRQ0 stub (defined below, links to irq0_handler)
extern void irq0_stub();
extern void irq1_stub();

// Initialize IDT
void idt_init() {
  // Clear IDT
  for (int i = 0; i < 256; i++) {
    idt_set_gate(i, 0, 0, 0);
  }

  // Set up IRQ0 handler (INT 32 after PIC remap)
  idt_set_gate(32, (u32)irq0_stub, 0x08,
               0x8E); // 0x8E = Present, Ring 0, 32-bit Interrupt Gate

  // Set up IRQ1 handler (INT 33 after PIC remap) - Keyboard
  idt_set_gate(33, (u32)irq1_stub, 0x08, 0x8E);

  // Load IDT
  idt_ptr.limit = sizeof(idt_entries) - 1;
  idt_ptr.base = (u32)&idt_entries;
  asm volatile("lidt %0" : : "m"(idt_ptr));

  // Initialize PIC and PIT
  pic_remap();
  pit_init();

  // Enable interrupts
  asm volatile("sti");
}

// Keyboard IRQ state for input.cpp to read
volatile u8 g_last_scancode = 0;
volatile bool g_scancode_ready = false;

// IRQ1 (Keyboard) Handler
void irq1_handler() {
  // Read scancode to acknowledge the interrupt
  g_last_scancode = inb(0x60);
  g_scancode_ready = true;

  // Send EOI to PIC
  outb(PIC1_CMD, 0x20);
}

} // extern "C"
