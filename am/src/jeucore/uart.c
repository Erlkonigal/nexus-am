#include <am.h>

#ifndef JEUCORE_UART_BASE
#error JEUCORE_UART_BASE is required
#endif

enum { RBR_THR_DLL = 0, IER_DLH = 4, FCR = 8, LCR = 12, LSR = 20 };

static volatile uint32_t *reg(uintptr_t offset) {
  return (volatile uint32_t *)(JEUCORE_UART_BASE + offset);
}

void jeucore_uart_init(void) {
  uint32_t divisor = (JEUCORE_UART_CLOCK_HZ + 8 * JEUCORE_UART_BAUD) / (16 * JEUCORE_UART_BAUD);
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

int jeucore_getc(void) {
  while ((*reg(LSR) & 1) == 0) {}
  return *reg(RBR_THR_DLL) & 0xff;
}
