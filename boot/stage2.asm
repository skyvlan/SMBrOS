; ============================================================================
; SMBrOS Stage 2 - Universal (Clean)
; ============================================================================
bits 16
org 0x7E00

KERNEL_LOAD_SEG     equ 0x1000
KERNEL_START_SECTOR equ 9       ; LBA 9 (Sector 10)
BOOT_INFO_ADDR      equ 0x9000

start:
    ; Init Segments (Safe)
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7E00
    sti

    ; === DEBUG: Write '1' to screen (segment init done) ===
    push es
    mov ax, 0xB800
    mov es, ax
    mov word [es:0], 0x4F31      ; '1' in red on white
    pop es  

    mov [boot_drive], dl
    call init_serial

    ; === DEBUG: Write '2' to screen (serial init done) ===
    push es
    mov ax, 0xB800
    mov es, ax
    mov word [es:2], 0x4F32      ; '2'
    pop es

    mov si, msg_loading
    call print_string_bios

    ; ------------------------------------------------------------------------
    ; 1. Enable A20 (Robust)
    ; ------------------------------------------------------------------------
    call enable_a20_robust
    cmp ax, 1
    je .a20_ok
    mov si, msg_a20_fail
    call print_string_bios
    hlt
.a20_ok:
    ; === DEBUG: Write '3' to screen (A20 enabled) ===
    push es
    mov ax, 0xB800
    mov es, ax
    mov word [es:4], 0x4F33      ; '3'
    pop es

    ; ------------------------------------------------------------------------
    ; 2. Load Kernel (Hybrid LBA/CHS)
    ; ------------------------------------------------------------------------
.load_kernel:
    ; === DEBUG: Write 'L' to screen (about to check LBA) ===
    push es
    mov ax, 0xB800
    mov es, ax
    mov word [es:6], 0x4F4C      ; 'L'
    pop es

    ; Check LBA Support
    mov ah, 0x41
    mov bx, 0x55AA
    mov dl, [boot_drive]
    int 0x13
    jc .use_chs

    ; === DEBUG: Write '4' to screen (LBA supported, starting load) ===
    push es
    mov ax, 0xB800
    mov es, ax
    mov word [es:8], 0x4F34      ; '4'
    pop es

    ; --- LBA Path (HDD/USB) ---
    mov si, dap
    mov word [si], 0x10
    mov word [si+2], 64         ; Read 32KB chunks
    mov word [si+4], 0
    mov word [si+6], KERNEL_LOAD_SEG
    mov dword [si+8], KERNEL_START_SECTOR
    
    mov cx, 16                  ; Load 16 chunks (512KB)
.lba_loop:
    mov ah, 0x42
    mov dl, [boot_drive]
    mov si, dap
    int 0x13
    jc .disk_error
    
    add word [dap+6], 0x800     ; Next memory segment
    add dword [dap+8], 64       ; Next LBA

    ; === DEBUG: Write 'R' to screen (read chunk done) ===
    push es
    mov ax, 0xB800
    mov es, ax
    mov word [es:10], 0x4F52    ; 'R'
    pop es

    loop .lba_loop

    ; === DEBUG: Write '5' to screen (kernel loaded via LBA) ===
    push es
    mov ax, 0xB800
    mov es, ax
    mov word [es:12], 0x4F35    ; '5'
    pop es

    jmp .init_vesa

    ; --- CHS Path (Floppy Fallback) ---
.use_chs:
    ; === DEBUG: Write 'C' to screen (using CHS path) ===
    push es
    mov ax, 0xB800
    mov es, ax
    mov word [es:8], 0x4F43      ; 'C'
    pop es

    mov ax, KERNEL_LOAD_SEG
    mov es, ax
    xor bx, bx
    
    mov ax, KERNEL_START_SECTOR
    mov cx, 1000                ; Load ~500KB
    
    ; Reset Disk first
    xor ax, ax
    mov dl, [boot_drive]
    int 0x13

    ; === DEBUG: Write 'D' to screen (disk reset done) ===
    push es
    push ax
    mov ax, 0xB800
    mov es, ax
    mov word [es:10], 0x4F44     ; 'D'
    ; Also show boot drive number (0x00=floppy, 0x80+=HD)
    movzx ax, byte [boot_drive]
    add al, '0'                  ; Convert to ASCII digit
    cmp al, '9'
    jle .digit_ok
    add al, 7                    ; A-F for hex
.digit_ok:
    mov ah, 0x4F                 ; White on red
    mov word [es:12], ax         ; Show drive number
    pop ax
    pop es
    
    ; Start loading from sector 9
    mov ax, KERNEL_START_SECTOR
    ; Fall through to loop

.chs_loop:
    ; === DEBUG: Write 'S' at start of each read attempt ===
    push es
    push ax
    mov ax, 0xB800
    mov es, ax
    mov word [es:14], 0x4F53     ; 'S'
    pop ax
    pop es

    push cx
    push ax                     ; Preserve LBA sector number
    call lba_to_chs_floppy      ; Convert AX(LBA) -> CH/DH/CL
    
    mov di, 3                   ; Retry count
.read_retry:
    push di
    
    mov ah, 0x02
    mov al, 1
    mov dl, [boot_drive]
    int 0x13
    jnc .read_ok

    ; Fail
    pop di
    dec di
    jz .disk_error
    
    ; Reset and Retry
    push ax
    xor ax, ax
    mov dl, [boot_drive]
    int 0x13
    pop ax
    jmp .read_retry

.read_ok:
    pop di
    
    mov dx, es
    add dx, 0x20
    mov es, dx
    
    pop ax                      ; Restore LBA sector number
    inc ax                      ; Next sector
    pop cx
    loop .chs_loop

    ; ------------------------------------------------------------------------
    ; 3. VESA Initialization (Deferred)
    ; ------------------------------------------------------------------------
.init_vesa:
    ; Reset ES to 0 (CHS loading leaves ES pointing to last dest segment)
    xor ax, ax
    mov es, ax

%ifdef TEXT_MODE
    ; TEXT_MODE: Skip graphics, use 80x25 text mode for benchmarking
    jmp .text_mode_init
%endif
%ifdef FORCE_MODE13H
    ; Force Mode 13h for testing
    jmp .vesa_fail
%endif
    ; Get VBE Info
    mov ax, 0x4F00
    mov di, vbe_info_block
    int 0x10
    cmp ax, 0x004F
    jne .vesa_fail

    ; Get/Set Mode 101h (640x480x256)
    mov ax, 0x4F01
    mov cx, 0x101
    mov di, mode_info_block
    int 0x10
    cmp ax, 0x004F
    jne .vesa_fail

    mov ax, 0x4F02
    mov bx, 0x4101 ; + LFB
    int 0x10
    cmp ax, 0x004F
    jne .vesa_fail

    ; Save Info
    mov di, BOOT_INFO_ADDR
    mov dword [di], 0x72424D53
    mov eax, [mode_info_block + 0x28]
    mov [di + 4], eax
    mov ax, [mode_info_block + 0x10]
    mov [di + 8], ax
    mov ax, [mode_info_block + 0x12]
    mov [di + 10], ax
    mov ax, [mode_info_block + 0x14]
    mov [di + 12], ax
    mov al, [mode_info_block + 0x19]
    mov [di + 14], al

    call setup_vga_palette
    jmp .pm_switch

.vesa_fail:
    ; ==========================================================================
    ; Fallback: Mode 13h (320x200x256) - Simple and fast!
    ; ==========================================================================
    ; Note: Mode X was tested but plane switching overhead made it slower.
    ; Sticking with linear Mode 13h for better performance.
    mov ax, 0x0013
    int 0x10
    mov di, BOOT_INFO_ADDR
    mov dword [di], 0x72424D53      ; 'SMBr' - same signature for render.cpp
    mov dword [di + 4], 0xA0000     ; Mode 13h framebuffer
    mov word [di + 8], 320          ; bytes_per_line
    mov word [di + 10], 320         ; x_res
    mov word [di + 12], 200         ; y_res
    mov byte [di + 14], 8           ; bits_per_pixel
    mov byte [di + 15], 0           ; Mode flag: 0 = Linear
    jmp .pm_switch

.text_mode_init:
    ; ==========================================================================
    ; TEXT_MODE: VGA 80x25 text mode for benchmarking game logic
    ; ==========================================================================
    mov ax, 0x0003                  ; VGA Mode 3: 80x25 16-color text
    int 0x10
    
    ; Print startup message
    mov si, msg_textmode
    call print_string_bios
    
    ; Store text mode info
    mov di, BOOT_INFO_ADDR
    mov dword [di], 0x72424D53      ; 'SMBr'
    mov dword [di + 4], 0xB8000     ; Text mode VRAM
    mov word [di + 8], 160          ; bytes_per_line (80 chars * 2 bytes)
    mov word [di + 10], 80          ; x_res (columns)
    mov word [di + 12], 25          ; y_res (rows)
    mov byte [di + 14], 16          ; bits_per_pixel (dummy for text)
    mov byte [di + 15], 2           ; Mode flag: 2 = Text mode

.pm_switch:
    ; === DEBUG: Write '5' to screen (about to enter PM) ===
    push es
    mov ax, 0xB800
    mov es, ax
    mov word [es:8], 0x4F35      ; '5'
    pop es

    cli
    lgdt [gdt_desc]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:pm_entry

.disk_error:
    mov si, msg_disk_error
    call print_string_bios
    hlt

; ============================================================================
; Helpers
; ============================================================================
enable_a20_robust:
    ; =========================================================================
    ; First: Check if A20 is already enabled (avoid touching 8042 if possible)
    ; This preserves USB Legacy keyboard emulation
    ; =========================================================================
    call .test_a20
    jc .a20_done              ; A20 already on, skip all methods

    ; Method 1: BIOS INT 15h (safest)
    mov ax, 0x2401
    int 0x15
    call .test_a20
    jc .a20_done              ; Success!

    ; Method 2: Keyboard Controller (most compatible but may break USB Legacy)
    call .wait_kbd
    mov al, 0xAD            ; Disable keyboard
    out 0x64, al
    call .wait_kbd
    mov al, 0xD0            ; Read output port
    out 0x64, al
    call .wait_kbd_data
    in al, 0x60
    push ax
    call .wait_kbd
    mov al, 0xD1            ; Write output port
    out 0x64, al
    call .wait_kbd
    pop ax
    or al, 2                ; Set A20 bit
    out 0x60, al
    call .wait_kbd
    mov al, 0xAE            ; Enable keyboard
    out 0x64, al
    call .wait_kbd

    call .test_a20
    jc .a20_done              ; Success!

    ; Method 3: Fast A20 (Port 0x92) - needed on many 486 boards
    in al, 0x92
    or al, 2                ; Set A20 bit (bit 1)
    and al, 0xFE            ; Clear bit 0 (avoid system reset!)
    out 0x92, al

    call .test_a20
    jc .a20_done              ; Success!

    ; All methods failed
    xor ax, ax
    ret

.a20_done:
    mov ax, 1
    ret

; Test if A20 is enabled by comparing wraparound
; Returns: CF=1 if A20 enabled, CF=0 if disabled
.test_a20:
    push ds
    push es
    push di
    push si
    
    xor ax, ax
    mov es, ax              ; ES = 0x0000
    not ax
    mov ds, ax              ; DS = 0xFFFF
    
    mov di, 0x0500          ; ES:DI = 0000:0500
    mov si, 0x0510          ; DS:SI = FFFF:0510 = 0x100500 (with A20) or 0x0500 (without)
    
    mov al, [es:di]         ; Save original bytes
    push ax
    mov al, [ds:si]
    push ax
    
    mov byte [es:di], 0x00  ; Write different values
    mov byte [ds:si], 0xFF
    
    cmp byte [es:di], 0xFF  ; If equal, A20 is OFF (wrapped)
    
    pop ax
    mov [ds:si], al         ; Restore original bytes
    pop ax
    mov [es:di], al
    
    pop si
    pop di
    pop es
    pop ds
    
    je .a20_off
    stc                     ; A20 is ON
    ret
.a20_off:
    clc                     ; A20 is OFF
    ret

.wait_kbd:
    in al, 0x64
    test al, 2
    jnz .wait_kbd
    ret
    
.wait_kbd_data:
    in al, 0x64
    test al, 1
    jz .wait_kbd_data
    ret

lba_to_chs_floppy:
    ; =========================================================================
    ; LBA to CHS conversion using BIOS-reported geometry
    ; Input: AX = LBA sector number
    ; Output: CH = cylinder low, CL = sector (bits 0-5) + cyl high (bits 6-7)
    ;         DH = head, DL = drive
    ; Preserves: AX (returns same LBA value)
    ; =========================================================================
    
    ; First get drive geometry from BIOS if not cached
    cmp byte [geo_cached], 1
    je .do_conversion
    
    ; Need to get geometry - save registers
    push ax
    push bx
    push es
    push di
    
    ; INT 13h AH=08h - Get Drive Geometry
    xor di, di
    mov es, di                  ; ES:DI = 0000:0000 (some BIOSes need this)
    mov ah, 0x08
    mov dl, [boot_drive]
    int 0x13
    jc .use_floppy_defaults     ; If error, use floppy defaults
    
    ; Parse geometry from return values
    ; CL bits 0-5 = max sector, bits 6-7 = high bits of max cylinder
    ; DH = max head
    mov al, cl
    and al, 0x3F                ; Sectors per track (max sector number)
    mov [sectors_per_track], al
    inc dh                      ; DH is max head (0-based), we need count
    mov [num_heads], dh
    mov byte [geo_cached], 1
    jmp .restore_and_convert

.use_floppy_defaults:
    ; Use floppy defaults: 18 sectors, 2 heads
    mov byte [sectors_per_track], 18
    mov byte [num_heads], 2
    mov byte [geo_cached], 1
    
.restore_and_convert:
    pop di
    pop es
    pop bx
    pop ax
    
.do_conversion:
    ; Now do the LBA -> CHS conversion
    ; S = (LBA % sectors) + 1
    ; H = (LBA / sectors) % heads
    ; C = (LBA / sectors) / heads
    
    push ax                     ; Save original LBA for return
    push bx                     ; Save BX
    
    xor dx, dx
    movzx bx, byte [sectors_per_track]
    div bx                      ; AX = LBA / sectors, DX = LBA % sectors
    inc dl                      ; Sector = (LBA % sectors) + 1
    mov cl, dl                  ; CL = sector number
    
    xor dx, dx
    movzx bx, byte [num_heads]
    div bx                      ; AX = cylinder, DX = head
    mov dh, dl                  ; DH = head
    mov ch, al                  ; CH = cylinder (low 8 bits)
    
    ; Handle cylinder high bits (for cylinders > 255)
    ; CL bits 6-7 = cylinder bits 8-9
    shl ah, 6
    or cl, ah
    
    mov dl, [boot_drive]        ; DL = drive number
    
    pop bx                      ; Restore BX
    pop ax                      ; Restore original LBA
    ret

; Geometry cache
geo_cached:       db 0
sectors_per_track: db 18
num_heads:         db 2

init_serial:
    mov dx, 0x3F8 + 3
    mov al, 0x80
    out dx, al
    mov dx, 0x3F8
    mov al, 0x01
    out dx, al
    mov dx, 0x3F8 + 3
    mov al, 0x03
    out dx, al
    ret

print_string_bios:
    mov ah, 0x0E
.loop:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .loop
.done:
    ret

setup_vga_palette:
    mov dx, 0x3C8
    xor al, al
    out dx, al
    mov dx, 0x3C9
    out dx, al
    out dx, al
    out dx, al
    out dx, al
    out dx, al
    mov al, 63
    out dx, al
    ret

boot_drive     db 0
msg_loading    db 'Loading SMBrOS...', 13, 10, 0
msg_disk_error db 'Disk Error!', 0
msg_a20_fail   db 'A20 Fail!', 0
msg_textmode   db 'TEXT_MODE: Benchmarking game logic...', 13, 10, 0
dap times 16 db 0

gdt_start: dq 0
           dw 0xFFFF, 0, 0x9A00, 0x00CF
           dw 0xFFFF, 0, 0x9200, 0x00CF
gdt_desc:  dw $ - gdt_start - 1
           dd gdt_start

vbe_info_block:  times 512 db 0
mode_info_block: times 256 db 0

bits 32
pm_entry:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, 0x90000

    ; === DEBUG: Write '6' to screen (in PM, segments set) ===
    mov dword [0xB800A], 0x4F364F36    ; '66' to be sure we're in PM
    
    mov esi, 0x10000
    mov edi, 0x100000
    mov ecx, 0x20000
    rep movsd

    ; === DEBUG: Write '7' to screen (kernel copied to 1MB) ===
    mov dword [0xB800E], 0x4F374F37    ; '77'
    
    ; === DEBUG: Write '8' before jump ===
    mov dword [0xB8012], 0x4F384F38    ; '88'

    jmp 0x100000

