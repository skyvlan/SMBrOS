; =============================================================================
; hal/tile_asm.asm - Fast tile renderer (Nesticle-style)
; =============================================================================
; Draws an 8x8 pre-decoded tile to the internal buffer.
;
; Standard cdecl calling convention (for non-regparm builds):
;   [ebp+8]  = tile pointer (64 bytes, pre-decoded)
;   [ebp+12] = dest pointer (256 bytes per row)
;   [ebp+16] = palette pointer (4 bytes)
;   [ebp+20] = bg_color

section .text
global draw_tile_fast

draw_tile_fast:
    push    ebp
    mov     ebp, esp
    push    ebx
    push    esi
    push    edi

    ; Standard cdecl: all args on stack
    mov     esi, [ebp+8]        ; ESI = tile pointer
    mov     edi, [ebp+12]       ; EDI = dest pointer  
    mov     edx, [ebp+16]       ; EDX = palette pointer
    movzx   ebx, byte [ebp+20]  ; EBX = bg_color

    mov     ecx, 8              ; 8 rows

.row_loop:
    push    ecx                 ; Save row counter

    ; --- Pixel 0 ---
    movzx   eax, byte [esi+0]
    test    al, al
    jz      .p0_bg
    movzx   eax, byte [edx+eax]
    and     al, 0x3F
    jmp     .p0_write
.p0_bg:
    mov     eax, ebx
.p0_write:
    mov     [edi+0], al

    ; --- Pixel 1 ---
    movzx   eax, byte [esi+1]
    test    al, al
    jz      .p1_bg
    movzx   eax, byte [edx+eax]
    and     al, 0x3F
    jmp     .p1_write
.p1_bg:
    mov     eax, ebx
.p1_write:
    mov     [edi+1], al

    ; --- Pixel 2 ---
    movzx   eax, byte [esi+2]
    test    al, al
    jz      .p2_bg
    movzx   eax, byte [edx+eax]
    and     al, 0x3F
    jmp     .p2_write
.p2_bg:
    mov     eax, ebx
.p2_write:
    mov     [edi+2], al

    ; --- Pixel 3 ---
    movzx   eax, byte [esi+3]
    test    al, al
    jz      .p3_bg
    movzx   eax, byte [edx+eax]
    and     al, 0x3F
    jmp     .p3_write
.p3_bg:
    mov     eax, ebx
.p3_write:
    mov     [edi+3], al

    ; --- Pixel 4 ---
    movzx   eax, byte [esi+4]
    test    al, al
    jz      .p4_bg
    movzx   eax, byte [edx+eax]
    and     al, 0x3F
    jmp     .p4_write
.p4_bg:
    mov     eax, ebx
.p4_write:
    mov     [edi+4], al

    ; --- Pixel 5 ---
    movzx   eax, byte [esi+5]
    test    al, al
    jz      .p5_bg
    movzx   eax, byte [edx+eax]
    and     al, 0x3F
    jmp     .p5_write
.p5_bg:
    mov     eax, ebx
.p5_write:
    mov     [edi+5], al

    ; --- Pixel 6 ---
    movzx   eax, byte [esi+6]
    test    al, al
    jz      .p6_bg
    movzx   eax, byte [edx+eax]
    and     al, 0x3F
    jmp     .p6_write
.p6_bg:
    mov     eax, ebx
.p6_write:
    mov     [edi+6], al

    ; --- Pixel 7 ---
    movzx   eax, byte [esi+7]
    test    al, al
    jz      .p7_bg
    movzx   eax, byte [edx+eax]
    and     al, 0x3F
    jmp     .p7_write
.p7_bg:
    mov     eax, ebx
.p7_write:
    mov     [edi+7], al

    ; Advance to next row
    add     esi, 8              ; Next tile row (8 bytes)
    add     edi, 512            ; Next screen row (512 bytes)
    
    pop     ecx                 ; Restore row counter
    dec     ecx
    jnz     .row_loop

    pop     edi
    pop     esi
    pop     ebx
    pop     ebp
    ret
