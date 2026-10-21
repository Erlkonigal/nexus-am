# cosim AM smoke

Build with `AM_HOME` set to this repository's absolute path:

```sh
make ARCH=riscv64-cosim CROSS_COMPILE=riscv64-unknown-elf-
make ARCH=riscv64-cosim CROSS_COMPILE=riscv64-unknown-elf- \
  COSIM_DUT=/path/to/Jeucore/build/sim/libjeucore_sim.so run
```

The application exercises klib formatting, strings and allocation along with UART
input and completion handling. Input `Z` prints `wave 42` and `OK`, then returns
through the simulator pass address; other input prints `FAIL` and returns through
the fail address. Override `COSIM_UART_INPUT` to change the input. `COSIM_REF`
defaults to the sibling `riscv-isa-sim/build-cosim/libspike_cosim.so`; set it to
an empty value to disable lockstep. The runner receives its configuration through
environment variables and does not create a configuration file. The smoke target
delivers the first input byte after 100 UART device cycles so firmware UART
initialization has completed; override `COSIM_UART_INPUT_DELAY_CYCLES` to change
the initial delay and the interval between subsequent input bytes.
