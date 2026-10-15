NAME = jeucore-smoke
SRCS = src/main.c
JEUCORE_SIM_ARGS ?= +uart-input=Z +wave=0 +counters=0 +trace=none
include $(AM_HOME)/Makefile.app

check: image
	python3 test.py --elf $(BINARY).elf --sim $(JEUCORE_SIM) \
	  --readelf $(READELF) --nm $(CROSS_COMPILE)nm \
	  --ram-base $(JEUCORE_RAM_BASE) --ram-size $(JEUCORE_RAM_SIZE) \
	  --pass-pc $(JEUCORE_SIM_PASS_ADDR) --fail-pc $(JEUCORE_SIM_FAIL_ADDR)

.PHONY: check
