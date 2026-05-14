; ============================================================================
; kernel/irq_stubs.asm - IRQ Handler Stubs
; ============================================================================
bits 32
section .text

; External C handler
extern irq0_handler

; IRQ0 Stub - saves registers, calls C handler, restores, returns
global irq0_stub
irq0_stub:
    pusha           ; Save all general purpose registers
    
    call irq0_handler
    
    popa            ; Restore registers
    iret            ; Return from interrupt

; IRQ1 Stub (Keyboard) - reads scancode and sends EOI
; Helps keep USB Legacy SMM emulation happy
extern irq1_handler
global irq1_stub
irq1_stub:
    pusha
    
    call irq1_handler
    
    popa
    iret
