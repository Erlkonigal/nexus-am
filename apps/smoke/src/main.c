#include <am.h>
#include <cosim.h>
#include <klib.h>

int main(const char *args) {
  (void)args;
  if (cosim_getc() != 'Z') goto fail;

  char *buffer = malloc(32);
  if (!buffer) goto fail;
  strcpy(buffer, "wave");
  printf("%s %d\n", buffer, 42);
  if (strcmp(buffer, "wave") != 0) goto fail;
  free(buffer);

  char formatted[32];
  if (snprintf(formatted, sizeof(formatted), "%s %02x", "value", 42) != 8 ||
      strcmp(formatted, "value 2a") != 0) goto fail;
  char moved[8] = "abcdef";
  memmove(moved + 1, moved, 5);
  if (strcmp(moved, "aabcde") != 0) goto fail;

  int *value = malloc(sizeof(*value));
  if (!value || ((uintptr_t)value & 15)) goto fail;
  *value = 42;
  value = realloc(value, 32);
  if (!value || *value != 42) goto fail;
  free(value);
  int *zeros = calloc(4, sizeof(int));
  if (!zeros || zeros[3] != 0) goto fail;
  free(zeros);

  _putc('O'); _putc('K'); _putc('\n');
  return 0;

fail:
  _putc('F'); _putc('A'); _putc('I'); _putc('L'); _putc('\n');
  return 1;
}
