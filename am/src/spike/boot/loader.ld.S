ENTRY(_start)
MEMORY {
  ram (rwx) : ORIGIN = SPIKE_RAM_BASE, LENGTH = SPIKE_RAM_SIZE
}
SECTIONS {
  .reset SPIKE_RAM_BASE : { KEEP(*(.reset)) } > ram
  . = ALIGN(64);
  .tohost : {
    tohost = .;
    . += 8;
    fromhost = .;
    . += 8;
  } > ram
  . = ALIGN(16);
  .text : { *(.text.startup) *(.text .text.*) } > ram
  .rodata : { *(.rodata .rodata.*) } > ram
  .data : { *(.data .data.*) } > ram
  . = ALIGN(8);
  __global_pointer$ = . + 0x800;
  .bss : {
    . = ALIGN(8);
    __bss_start = .;
    *(.sbss .sbss.*) *(.bss .bss.*) *(COMMON)
    . = ALIGN(8);
    __bss_end = .;
  } > ram
  __heap_start = ALIGN(16);
  __stack_top = ORIGIN(ram) + LENGTH(ram);
  __heap_end = __stack_top - 0x8000;
  ASSERT(__heap_start < __heap_end, "Spike program exceeds RAM")
}
