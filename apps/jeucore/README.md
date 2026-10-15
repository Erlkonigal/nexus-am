# Jeucore AM smoke

Build with `AM_HOME` set to this repository's absolute path:

```sh
make ARCH=riscv64-jeucore CROSS_COMPILE=riscv64-unknown-elf-
make ARCH=riscv64-jeucore CROSS_COMPILE=riscv64-unknown-elf- \
  JEUCORE_SIM=/path/to/Jeucore/build/VSoC run
make ARCH=riscv64-jeucore CROSS_COMPILE=riscv64-unknown-elf- \
  JEUCORE_SIM=/path/to/Jeucore/build/VSoC check
```

The application checks UART input and completion handling. Input `Z` prints `OK` and
returns through the simulator pass address; other input prints `FAIL` and returns
through the fail address. Override `JEUCORE_SIM_ARGS` to change the input or
simulator options.
