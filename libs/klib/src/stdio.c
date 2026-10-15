#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
typedef struct { char *out; size_t size, used; int count; } Output;

static void emit(Output *o, char c) {
  if (o->out && o->used + 1 < o->size) o->out[o->used++] = c;
  else if (o->out && o->size) o->used = o->size - 1;
  else if (!o->out) _putc(c);
  ++o->count;
}

static void number(Output *o, unsigned long long value, unsigned base, int negative, int width, char pad) {
  char digits[32]; int n = 0;
  do {
    unsigned digit = value % base;
    digits[n++] = digit < 10 ? '0' + digit : 'a' + digit - 10;
    value /= base;
  } while (value);
  if (negative && pad == '0') emit(o, '-');
  while (width-- > n + negative) emit(o, pad);
  if (negative && pad != '0') emit(o, '-');
  while (n) emit(o, digits[--n]);
}

static int format(Output *o, const char *fmt, va_list ap) {
  for (; *fmt; ++fmt) {
    if (*fmt != '%') { emit(o, *fmt); continue; }
    ++fmt;
    if (*fmt == '%') { emit(o, '%'); continue; }
    char pad = ' ';
    if (*fmt == '0') { pad = '0'; ++fmt; }
    int width = 0;
    while (*fmt >= '0' && *fmt <= '9') { width = width * 10 + *fmt++ - '0'; }
    int long_count = 0;
    while (*fmt == 'l') { ++long_count; ++fmt; }
    if (*fmt == 's') {
      const char *s = va_arg(ap, const char *);
      if (!s) s = "(null)";
      while (*s) emit(o, *s++);
    } else if (*fmt == 'c') {
      emit(o, (char)va_arg(ap, int));
    } else if (*fmt == 'd' || *fmt == 'i') {
      long long value = long_count > 1 ? va_arg(ap, long long) :
        long_count ? va_arg(ap, long) : va_arg(ap, int);
      unsigned long long magnitude = value < 0 ? 0ULL - (unsigned long long)value : (unsigned long long)value;
      number(o, magnitude, 10, value < 0, width, pad);
    } else if (*fmt == 'u' || *fmt == 'x' || *fmt == 'p') {
      unsigned long long value = *fmt == 'p' ? (uintptr_t)va_arg(ap, void *) :
        long_count > 1 ? va_arg(ap, unsigned long long) :
        long_count ? va_arg(ap, unsigned long) : va_arg(ap, unsigned int);
      if (*fmt == 'p') { emit(o, '0'); emit(o, 'x'); }
      number(o, value, *fmt == 'x' || *fmt == 'p' ? 16 : 10, 0, width, pad);
    } else if (*fmt) { emit(o, '%'); emit(o, *fmt); }
    else break;
  }
  if (o->out && o->size) o->out[o->used] = 0;
  return o->count;
}

int printf(const char *fmt, ...) {
  va_list ap; va_start(ap, fmt);
  Output o = {0}; int n = format(&o, fmt, ap);
  va_end(ap); return n;
}
int vsprintf(char *out, const char *fmt, va_list ap) {
  Output o = { .out = out, .size = (size_t)-1 };
  return format(&o, fmt, ap);
}
int sprintf(char *out, const char *fmt, ...) {
  va_list ap; va_start(ap, fmt);
  int n = vsprintf(out, fmt, ap);
  va_end(ap); return n;
}
int vsnprintf(char *out, size_t size, const char *fmt, va_list ap) {
  Output o = { .out = out, .size = size };
  return format(&o, fmt, ap);
}
int snprintf(char *out, size_t size, const char *fmt, ...) {
  va_list ap; va_start(ap, fmt);
  int n = vsnprintf(out, size, fmt, ap);
  va_end(ap); return n;
}
#endif
