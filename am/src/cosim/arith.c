#include <stdint.h>

uint64_t __muldi3(uint64_t a, uint64_t b) {
  uint64_t result = 0;
  while (b) {
    if (b & 1) result += a;
    a <<= 1;
    b >>= 1;
  }
  return result;
}

static uint64_t divide(uint64_t dividend, uint64_t divisor, uint64_t *remainder) {
  uint64_t quotient = 0;
  if (!divisor) { if (remainder) *remainder = 0; return 0; }
  uint64_t bit = 1;
  while (divisor <= (UINT64_MAX >> 1) && (divisor << 1) <= dividend) {
    divisor <<= 1;
    bit <<= 1;
  }
  while (bit) {
    if (dividend >= divisor) { dividend -= divisor; quotient |= bit; }
    divisor >>= 1;
    bit >>= 1;
  }
  if (remainder) *remainder = dividend;
  return quotient;
}

uint64_t __udivdi3(uint64_t a, uint64_t b) { return divide(a, b, 0); }
uint64_t __umoddi3(uint64_t a, uint64_t b) { uint64_t r; divide(a, b, &r); return r; }
int64_t __divdi3(int64_t a, int64_t b) {
  int negative = (a < 0) != (b < 0);
  uint64_t ua = a < 0 ? 0ULL - (uint64_t)a : (uint64_t)a;
  uint64_t ub = b < 0 ? 0ULL - (uint64_t)b : (uint64_t)b;
  uint64_t q = divide(ua, ub, 0);
  return (int64_t)(negative ? 0ULL - q : q);
}
int64_t __moddi3(int64_t a, int64_t b) {
  uint64_t ua = a < 0 ? 0ULL - (uint64_t)a : (uint64_t)a;
  uint64_t ub = b < 0 ? 0ULL - (uint64_t)b : (uint64_t)b;
  uint64_t r; divide(ua, ub, &r);
  return (int64_t)(a < 0 ? 0ULL - r : r);
}
unsigned __udivsi3(unsigned a, unsigned b) { return (unsigned)divide(a, b, 0); }
unsigned __umodsi3(unsigned a, unsigned b) { uint64_t r; divide(a, b, &r); return (unsigned)r; }
int __divsi3(int a, int b) { return (int)__divdi3(a, b); }
int __modsi3(int a, int b) { return (int)__moddi3(a, b); }
