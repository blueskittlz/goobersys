; bootloader.s - 512 bytes boot sector
[org 0x7c00]

; -------------------------
; Stage 1: Set up GDT
; -------------------------

; GDT data
gdt_start:
    dq 0x0000000000000000  ; Null descriptor
    dq 0x00af9a000000ffff  ; Code segment descriptor
    dq 0x00cf92000000ffff  ; Data segment descriptor
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; -------------------------
; Code starts here
; -------------------------

start:
    cli                     ; Disable interrupts
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00          ; Stack pointer

    lgdt [gdt_descriptor]   ; Load GDT

    ; Enable protected mode (set PE bit in CR0)
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump to flush pipeline and load CS with code segment selector (0x08)
    jmp 0x08:protected_mode_entry

; -------------------------
; Protected mode entry point
; -------------------------
protected_mode_entry:
    ; Set up segment registers
    mov ax, 0x10            ; Data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000        ; Set up stack in protected mode

    ; Load kernel from disk
    mov bx, 0x0000          ; Offset to load kernel (0x1000:0x0000 = 0x10000)
    mov dh, 2               ; Number of sectors to read
    call load_kernel

    ; Jump to kernel (loaded at 0x10000)
    jmp 0x1000:0x0000

; -------------------------
; Disk read routine (BIOS interrupt 13h)
; -------------------------
load_kernel:
    mov ah, 0x02            ; BIOS read sectors function
    mov al, dh              ; Number of sectors to read
    mov ch, 0x00            ; Cylinder
    mov cl, 0x02            ; Start reading at sector 2 (sector 1 is bootloader)
    mov dh, 0x00            ; Head
    mov dl, 0x80            ; Drive (first HDD)
    mov bx, bx              ; ES:BX points to memory buffer for disk read
    int 0x13
    jc disk_error
    ret

disk_error:
    ; Handle error (just hang here)
    cli
    hlt
    jmp $

; -------------------------
; Bootloader signature (must be last 2 bytes)
; -------------------------
times 510-($-$$) db 0
dw 0xAA55
