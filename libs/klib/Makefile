NAME = klib
SRCS = src/stdio.c \
       src/string.c \
       src/cpp.c \
       src/stdlib.c \
       src/io.c \
       src/int64.c
ifeq ($(ARCH),riscv64-cosim)
SRCS := $(filter-out src/int64.c,$(SRCS))
endif
include $(AM_HOME)/Makefile.lib
