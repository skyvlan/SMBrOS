/**
 * SMBrOS C++ Kernel Linker Script
 * Based on hal/multiboot.ld
 */

OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS
{
    /* Kernel starts at 1MB */
    . = 1M;
    
    .boot : ALIGN(4)
    {
        *(.boot)
    }
    
    .text : ALIGN(4K)
    {
        *(.text)
        *(.text.*)
    }
    
    .rodata : ALIGN(4K)
    {
        *(.rodata)
        *(.rodata.*)
    }
    
    .data : ALIGN(4K)
    {
        *(.data)
        *(.data.*)
    }
    
    .bss : ALIGN(4K)
    {
        __bss_start = .;
        *(COMMON)
        *(.bss)
        *(.bss.*)
        __bss_end = .;
    }
    
    _kernel_end = .;
}
