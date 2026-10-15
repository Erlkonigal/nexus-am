#include <klib.h>

static unsigned char arena[4096];
_Area _heap = { arena, arena + sizeof(arena) };
static char console[32];
static size_t console_size;

void _putc(char ch) { if (console_size < sizeof(console)) console[console_size++] = ch; }
void _halt(int code) { (void)code; for (;;) {} }

int main(void) {
  char out[32];
  if (snprintf(out, sizeof(out), "%s %d %02x", "OK", -42, 10) != 9) return 1;
  if (strcmp(out, "OK -42 0a") != 0) return 2;
  if (snprintf(out, 4, "hello") != 5 || strcmp(out, "hel") != 0) return 3;
  if (printf("%c %u", 'A', 7) != 3 || console_size != 3 || memcmp(console, "A 7", 3)) return 4;

  char text[8] = "abcdef";
  memmove(text + 1, text, 5);
  if (strcmp(text, "aabcde") != 0 || strlen(text) != 6) return 5;

  int *value = malloc(sizeof(*value));
  if (!value || ((uintptr_t)value & 15)) return 6;
  *value = 42;
  value = realloc(value, 32);
  if (!value || *value != 42) return 7;
  free(value);
  int *zeros = calloc(4, sizeof(int));
  if (!zeros || zeros[3] != 0) return 8;
  free(zeros);
  return 0;
}
