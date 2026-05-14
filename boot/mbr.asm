; ============================================================================
; SMBrOS MBR - Real Hardware Compatible (Clean)
; ============================================================================
bits 16
org 0x7C00

start:
    jmp short init      ; Jump over BPB
    nop

    ; --- BIOS Parameter Block (BPB) Reserve ---
    times 59 db 0

init:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    ; 1. Save Drive Number
    mov [boot_drive], dl

    ; 2. Reset Disk System
    xor ax, ax
    mov dl, [boot_drive]
    int 0x13

    ; 3. Load Stage 2
    ; Try LBA First (Essential for USB-HDD)
.try_lba:
    ; Check Extensions Present
    mov ah, 0x41
    mov bx, 0x55AA
    mov dl, [boot_drive]
    int 0x13
    jc .read_chs_loop       ; No LBA -> Fallback
    cmp bx, 0xAA55
    jne .read_chs_loop

    ; Setup LBA Packet
    mov si, disk_packet
    mov word [si], 0x10     ; Size 16 bytes
    mov word [si+2], 8      ; Read 8 sectors (Stage 2)
    mov word [si+4], 0x7E00 ; Offset
    mov word [si+6], 0      ; Segment
    mov dword [si+8], 1     ; Start LBA (Sector 1 = Stage 2)
    mov dword [si+12], 0    ; High LBA

    mov ah, 0x42
    mov dl, [boot_drive]
    int 0x13
    jnc .success

.read_chs_loop:
    mov di, 3           ; Retry count
.chs_retry:
    push di
    
    mov ah, 0x02
    mov al, 8           ; Sectors to read
    mov ch, 0           ; Cylinder 0
    mov cl, 2           ; Start Sector 2
    mov dh, 0           ; Head 0
    mov dl, [boot_drive]
    mov bx, 0x7E00      ; Dest
    int 0x13
    
    jnc .success_pop

    ; Reset and Retry
    xor ax, ax
    int 0x13
    pop di
    dec di
    jnz .chs_retry

    jmp .disk_error

.success_pop:
    pop di
.success:
    
    ; 4. Jump to Stage 2
    mov dl, [boot_drive]
    jmp 0x0000:0x7E00

.disk_error:
    mov ah, 0x0E
    mov al, '!'
    int 0x10
    cli
    hlt

boot_drive db 0
disk_packet times 16 db 0
times 510-($-$$) db 0
dw 0xAA55
