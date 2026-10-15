#include <am.h>
#include <jeucore.h>

int main(const char *args) {
  (void)args;
  int input = jeucore_getc();
  if (input == 'Z') {
    _putc('O'); _putc('K'); _putc('\n');
    return 0;
  }
  _putc('F'); _putc('A'); _putc('I'); _putc('L'); _putc('\n');
  return 1;
}
