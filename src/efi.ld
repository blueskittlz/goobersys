ENTRY(efi_main)

SECTIONS {
  . = 0;
  .text : ALIGN(8) { *(.text*) }
  .data : ALIGN(8) { *(.data*) }
  .bss  : ALIGN(8) { *(.bss*) }
}
