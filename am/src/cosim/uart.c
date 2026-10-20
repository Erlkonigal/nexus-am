#include <am.h>

#ifndef COSIM_UART_BASE
#error COSIM_UART_BASE is required
#endif

enum { RBR_THR_DLL = 0, IER_DLH = 4, FCR = 8, LCR = 12, LSR = 20 };

static volatile uint32_t *reg(uintptr_t offset) {
  return (volatile uint32_t *)(COSIM_UART_BASE + offset);
}

void cosim_uart_init(void) {
  uint32_t divisor = (COSIM_UART_CLOCK_HZ + 8 * COSIM_UART_BAUD) / (16 * COSIM_UART_BAUD);
  if (divisor == 0) divisor = 1;
  if (divisor > 0xffff) divisor = 0xffff;
  *reg(IER_DLH) = 0;
  *reg(LCR) = 0x80;
  *reg(RBR_THR_DLL) = divisor & 0xff;
  *reg(IER_DLH) = divisor >> 8;
  *reg(LCR) = 3;
  *reg(FCR) = 7;
}

void _putc(char ch) {
  *reg(RBR_THR_DLL) = (unsigned char)ch;
}

int cosim_getc(void) {
  while ((*reg(LSR) & 1) == 0) {}
  return *reg(RBR_THR_DLL) & 0xff;
}
