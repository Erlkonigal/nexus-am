#include <am.h>

extern char __heap_start[], __heap_end[];
extern volatile uint64_t tohost;
int main(const char *args);
void spike_uart_init(void);

_Area _heap = { __heap_start, __heap_end };

void _trm_init(void) {
  spike_uart_init();
  _halt(main(""));
}

void _halt(int code) {
  tohost = ((uint64_t)(unsigned)code << 1) | 1;
  while (1) {}
}
