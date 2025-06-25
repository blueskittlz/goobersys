[BITS 16]           ; 16-bit mode for bootloader stage

; --------------------------
; GDT definitions & setup
; --------------------------

gdt_start:
    ; Null descriptor (required)
    dq 0x0000000000000000

    ; Code segment descriptor
    ; Base=0, Limit=4GB, Executable, Readable, Code segment, DPL=0, Granularity=4KB
    dq 0x00CF9A000000FFFF

    ; Data segment descriptor
    ; Base=0, Limit=4GB, Read/Write, Data segment, DPL=0, Granularity=4KB
    dq 0x00CF92000000FFFF

    ; User mode code segment (DPL=3)
    dq 0x00CFFA000000FFFF

    ; User mode data segment (DPL=3)
    dq 0x00CFF2000000FFFF

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1    ; Limit (size of GDT - 1)
    dd gdt_start                  ; Base address of GDT

; --------------------------
; Load GDT & switch to protected mode
; --------------------------

; Disable interrupts
cli

; Load GDT
lgdt [gdt_descriptor]

; Enable protected mode (set PE bit in CR0)
mov eax, cr0
or eax, 1
mov cr0, eax

; Far jump to flush instruction queue & load CS
jmp 08h:protected_mode_entry

[BITS 32]           ; Switch to 32-bit mode after jump

protected_mode_entry:
    ; Set up segment registers
    mov ax, 10h          ; Data segment selector (index 2 in GDT, descriptor 0x10)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Set up stack pointer
    mov esp, 0x9FC00     ; Example stack pointer

    ; Enable interrupts
    sti

    ; Now protected mode is ready, continue with kernel...
