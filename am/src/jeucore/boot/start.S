  .section .reset, "ax", @progbits
  .globl _start
  .option push
  .option norvc
  .option norelax
_start:
  j __jeucore_start
  .size _start, 4
  .option pop

  .section .sim_pass, "ax", @progbits
  .globl _sim_pass
  .option push
  .option norvc
_sim_pass:
  j _sim_pass
  .option pop

  .section .sim_fail, "ax", @progbits
  .globl _sim_fail
  .option push
  .option norvc
_sim_fail:
  j _sim_fail
  .option pop

  .section .text.startup, "ax", @progbits
__jeucore_start:
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
