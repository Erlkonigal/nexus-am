#include <am.h>

#ifndef SPIKE_UART_BASE
#error SPIKE_UART_BASE is required
#endif

enum { RBR_THR_DLL = 0, IER_DLH = 4, FCR = 8, LCR = 12, LSR = 20 };

static volatile uint32_t *reg(uintptr_t offset) {
  return (volatile uint32_t *)(SPIKE_UART_BASE + offset);
}

void spike_uart_init(void) {
  *reg(IER_DLH) = 0;
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
