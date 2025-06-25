[BITS 32]

section .data
idt_start:
    times 256 dq 0              ; 256 IDT entries, each 8 bytes, zeroed
idt_end:

idt_descriptor:
    dw idt_end - idt_start - 1  ; Limit (size of IDT - 1)
    dd idt_start                ; Base address of IDT

section .text
global load_idt
load_idt:
    cli                         ; Disable interrupts
    lidt [idt_descriptor]       ; Load IDT pointer
    sti                         ; Enable interrupts
    ret
