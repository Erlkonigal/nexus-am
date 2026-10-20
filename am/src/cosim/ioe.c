#include <am.h>

int _ioe_init(void) { return 0; }
size_t _io_read(uint32_t dev, uintptr_t reg, void *buf, size_t size) {
  (void)dev; (void)reg; (void)buf; (void)size;
  return 0;
}
size_t _io_write(uint32_t dev, uintptr_t reg, void *buf, size_t size) {
  (void)dev; (void)reg; (void)buf; (void)size;
  return 0;
}
