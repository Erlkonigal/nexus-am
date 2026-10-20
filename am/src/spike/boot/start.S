  .section .reset, "ax", @progbits
  .globl _start
  .option push
  .option norvc
  .option norelax
_start:
  j __spike_start
  .size _start, 4
  .option pop

  .section .text.startup, "ax", @progbits
__spike_start:
  .option push
  .option norelax
  la gp, __global_pointer$
  .option pop
  la sp, __stack_top
  la t0, __bss_start
  la t1, __bss_end
1:
  bgeu t0, t1, 2f
  sd zero, 0(t0)
  addi t0, t0, 8
  j 1b
2:
  call _trm_init
3:
  j 3b
