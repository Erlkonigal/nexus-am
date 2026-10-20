AM_HOME ?= $(CURDIR)
BEAR ?= bear
BEAR_DIR ?= apps/smoke
ARCH ?= riscv64-cosim

include Makefile.check

default:
	@$(MAKE) -s -C am

bear:
	$(BEAR) --output "$(CURDIR)/compile_commands.json" -- \
	  $(MAKE) -B -C "$(BEAR_DIR)" AM_HOME="$(AM_HOME)" ARCH="$(ARCH)"

# clean everything
ALLMAKE = $(dir $(shell find . -mindepth 2 -name "Makefile"))
$(ALLMAKE):
	-@$(MAKE) -s -C $@ clean

clean: $(ALLMAKE)

.PHONY: default bear clean $(ALLMAKE)
