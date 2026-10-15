CC ?= cc
CFLAGS := -std=gnu11 -O2 -Wall -Wextra -Werror -fno-builtin \
  -I../../am -I../../am/include -I../../libs/klib/include \
  -DARCH_H='"arch/native.h"' -D__ISA_NATIVE__ -D__NATIVE_USE_KLIB__ \
  -Dmalloc=am_malloc -Dfree=am_free -Dcalloc=am_calloc -Drealloc=am_realloc

check: build/host
	./build/host

build/host: host.c ../../libs/klib/src/string.c ../../libs/klib/src/stdio.c ../../libs/klib/src/stdlib.c
	@mkdir -p build
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf build

.PHONY: check clean
