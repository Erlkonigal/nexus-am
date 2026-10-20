SPIKE_HOME ?= $(AM_HOME)/../spike
SPIKE ?= $(SPIKE_HOME)/build/spike
SPIKE_ARGS ?= --isa=RV64IMAC
SPIKE_RAM_BASE ?= 0x80000000
SPIKE_RAM_SIZE ?= 0x10000000
SPIKE_UART_BASE ?= 0x10000000
SPIKE_UART_SIZE ?= 0x100

SPIKE_DEFINES := -DSPIKE_RAM_BASE=$(SPIKE_RAM_BASE) -DSPIKE_RAM_SIZE=$(SPIKE_RAM_SIZE) \
  -DSPIKE_UART_BASE=$(SPIKE_UART_BASE)
CFLAGS += $(SPIKE_DEFINES)
ASFLAGS += $(SPIKE_DEFINES)

AM_SRCS := spike/trm.c spike/uart.c spike/boot/start.S
LD_SCRIPT = $(DST_DIR)/spike.ld

image:
	@$(CC) $(COMMON_FLAGS) $(SPIKE_DEFINES) -E -P -x c \
	  $(AM_HOME)/am/src/spike/boot/loader.ld.S -o $(LD_SCRIPT)
	@echo + LD "->" $(BINARY_REL).elf
	@$(CC) $(LDFLAGS) -T $(LD_SCRIPT) -Wl,-Map,$(BINARY).map -o $(BINARY).elf \
	  -Wl,--start-group $(LINK_FILES) -Wl,--end-group
	@$(OBJDUMP) -d $(BINARY).elf > $(BINARY).txt

run: image
	$(SPIKE) $(SPIKE_ARGS) $(BINARY).elf
