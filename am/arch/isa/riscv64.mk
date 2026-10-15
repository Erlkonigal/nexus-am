CROSS_COMPILE ?= riscv64-unknown-elf-
COMMON_FLAGS := -fno-pic -march=rv64i -mabi=lp64 -mcmodel=medany -mno-relax
CFLAGS += $(COMMON_FLAGS) -ffreestanding -fdata-sections -ffunction-sections
ASFLAGS += $(COMMON_FLAGS)
LDFLAGS += $(COMMON_FLAGS) -nostdlib -static -Wl,--gc-sections,--no-relax,--build-id=none
