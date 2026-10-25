#include <am.h>

extern char __heap_start[], __heap_end[];
extern void _sim_pass(void) __attribute__((noreturn));
extern void _sim_fail(void) __attribute__((noreturn));
int main(const char *args);
extern char __mainargs[];
void cosim_uart_init(void);

_Area _heap = { __heap_start, __heap_end };

void _trm_init(void) {
  cosim_uart_init();
  _halt(main(__mainargs));
}

void _halt(int code) {
  if (code == 0) _sim_pass();
  _sim_fail();
}
