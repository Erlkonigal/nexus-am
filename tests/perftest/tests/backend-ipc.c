#include <am.h>

/*
 * Sustained backend-throughput probe.
 *
 * The hot body contains only independent compressed integer ALU operations.
 * Fourteen caller-saved registers are updated round-robin, which keeps every
 * operation independent.  A compact hot body reduces cold instruction-fetch
 * overhead; compressed instructions provide enough ALU operations to saturate
 * the backend.  With branch prediction, the loop backedge once per 112 ALU
 * operations needs little unrolling.  Total ALU work remains unchanged.
 */

#define INNER_REPEATS    1
#define OUTER_ITERATIONS (4096 * 64)
#define ACCUMULATORS     14
#define EXPECTED_SUM     (INNER_REPEATS * OUTER_ITERATIONS * ACCUMULATORS)

#define STRINGIFY_INNER(x) #x
#define STRINGIFY(x)       STRINGIFY_INNER(x)

__attribute__((noinline))
static unsigned long backend_ipc_block(void) {
  unsigned long checksum;

  __asm__ volatile(
      "li t0, 0\n\t"
      "li t1, 0\n\t"
      "li t2, 0\n\t"
      "li t3, 0\n\t"
      "li t4, 0\n\t"
      "li t5, 0\n\t"
      "li a0, 0\n\t"
      "li a1, 0\n\t"
      "li a2, 0\n\t"
      "li a3, 0\n\t"
      "li a4, 0\n\t"
      "li a5, 0\n\t"
      "li a6, 0\n\t"
      "li a7, 0\n\t"
      "li s0, " STRINGIFY(OUTER_ITERATIONS) "\n\t"
      "1:\n\t"
      ".option push\n\t"
      ".option rvc\n\t"
      ".rept " STRINGIFY(INNER_REPEATS) "\n\t"
      "c.addi t0, 1\n\t"
      "c.addi t1, 1\n\t"
      "c.addi t2, 1\n\t"
      "c.addi t3, 1\n\t"
      "c.addi t4, 1\n\t"
      "c.addi t5, 1\n\t"
      "c.addi a0, 1\n\t"
      "c.addi a1, 1\n\t"
      "c.addi a2, 1\n\t"
      "c.addi a3, 1\n\t"
      "c.addi a4, 1\n\t"
      "c.addi a5, 1\n\t"
      "c.addi a6, 1\n\t"
      "c.addi a7, 1\n\t"
      ".endr\n\t"
      "c.addi s0, -1\n\t"
      "c.bnez s0, 1b\n\t"
      ".option pop\n\t"
      "add t0, t0, t1\n\t"
      "add t0, t0, t2\n\t"
      "add t0, t0, t3\n\t"
      "add t0, t0, t4\n\t"
      "add t0, t0, t5\n\t"
      "add t0, t0, a0\n\t"
      "add t0, t0, a1\n\t"
      "add t0, t0, a2\n\t"
      "add t0, t0, a3\n\t"
      "add t0, t0, a4\n\t"
      "add t0, t0, a5\n\t"
      "add t0, t0, a6\n\t"
      "add t0, t0, a7\n\t"
      "mv %0, t0\n\t"
      : "=r"(checksum)
      :
      : "t0", "t1", "t2", "t3", "t4", "t5", "s0",
        "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "memory");

  return checksum;
}

int main(void) {
  return backend_ipc_block() != EXPECTED_SUM;
}
