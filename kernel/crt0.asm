; =============================================================================
; crt0.asm - C Runtime Entry Point
; =============================================================================
; This is the minimal C runtime that bridges the bootloader to C++ code.
; Entry point: _start (called by stage2 after protected mode switch)
; Calls: kernel_main() in kernel_main.cpp
; =============================================================================

BITS 32
SECTION .text

; External C++ function
extern kernel_main

; Global entry point
global _start
global _stop

; =============================================================================
; Entry Point
; =============================================================================
_start:
    ; Disable interrupts during initialization
    cli
    
    ; Set up C++ stack (grows downward from 2MB mark)
    mov esp, 0x200000
    mov ebp, esp
    
    ; Clear direction flag for string operations
    cld
    
    ; Clear BSS section
    extern __bss_start
    extern __bss_end
    mov edi, __bss_start
    mov ecx, __bss_end
    sub ecx, edi
    shr ecx, 2              ; Divide by 4 (clear dwords)
    xor eax, eax
    rep stosd
    
    ; Call C++ kernel main
    call kernel_main
    
    ; If kernel_main returns, halt
_stop:
    cli
    hlt
    jmp _stop

; =============================================================================
; Required C++ Runtime Stubs
; =============================================================================
; These are required by the C++ compiler but we don't need them

global __cxa_pure_virtual
__cxa_pure_virtual:
    ; Pure virtual function called - should never happen
    cli
    hlt
    jmp __cxa_pure_virtual

global __stack_chk_fail
__stack_chk_fail:
    ; Stack smashing detected - halt
    cli
    hlt
    jmp __stack_chk_fail

; =============================================================================
; BSS Section
; =============================================================================
SECTION .bss
    align 16
    resb 4096               ; Reserved stack space
