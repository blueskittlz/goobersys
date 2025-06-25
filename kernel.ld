ENTRY(kernel_main)

SECTIONS
{
  /* Place kernel code at physical address 0x10000 */
  . = 0x10000;

  .text :
  {
    *(.text.entry)
    *(.text*)
  }

  .rodata :
  {
    *(.rodata*)
  }

  .data :
  {
    *(.data*)
  }

  .bss :
  {
    *(.bss*)
    *(COMMON)
  }
}
