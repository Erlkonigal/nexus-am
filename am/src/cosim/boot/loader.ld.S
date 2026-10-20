ENTRY(_start)
MEMORY {
  ram (rwx) : ORIGIN = COSIM_RAM_BASE, LENGTH = COSIM_RAM_SIZE
}
SECTIONS {
  .reset COSIM_RAM_BASE : { KEEP(*(.reset)) } > ram
  .sim_pass COSIM_SIM_PASS_ADDR : { KEEP(*(.sim_pass)) } > ram
  .sim_fail COSIM_SIM_FAIL_ADDR : { KEEP(*(.sim_fail)) } > ram
  . = MAX(COSIM_RAM_BASE + 12, MAX(COSIM_SIM_PASS_ADDR + 4, COSIM_SIM_FAIL_ADDR + 4));
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
  ASSERT(__heap_start < __heap_end, "cosim program exceeds RAM")
}
