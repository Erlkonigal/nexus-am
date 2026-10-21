# nexus-am

## cosim 仿真平台

`ARCH=riscv64-cosim` 使用 RV64I、LP64 ABI 和 cosim SoC 的 RAM/UART 地址，生成可由
`cosim-run` 加载的 ELF。DUT 通过 `COSIM_DUT` 指定，因此平台与具体处理器实现无关。

```sh
make -C apps/smoke ARCH=riscv64-cosim CROSS_COMPILE=riscv64-unknown-elf-
make -C apps/smoke ARCH=riscv64-cosim CROSS_COMPILE=riscv64-unknown-elf- \
  COSIM_DUT=/path/to/Jeucore/build/sim/libjeucore_sim.so run
```

`run` 直接通过环境变量调用 `cosim-run`。平台地址和 UART 参数可通过
`COSIM_RAM_BASE`、`COSIM_RAM_SIZE`、`COSIM_UART_BASE`、`COSIM_UART_CLOCK_HZ`、
`COSIM_UART_BAUD`、`COSIM_SIM_PASS_ADDR` 和 `COSIM_SIM_FAIL_ADDR` 覆盖；输入字符由
`COSIM_UART_INPUT` 设置。`COSIM_REF` 默认指向 sibling
`riscv-isa-sim/build-cosim/libspike_cosim.so`，可在命令行设为空以关闭参考模型；
`COSIM_MAX_CYCLES` 和 `COSIM_MAX_INSTRUCTIONS` 只有显式设置时才限制运行。

在仓库根目录运行 `make bear` 可用 Bear 重编 `apps/smoke` 及其依赖，并生成
`compile_commands.json`。默认架构为 `riscv64-cosim`；可以指定重编目录、架构
和交叉编译器。

```sh
make bear BEAR_DIR=apps/smoke ARCH=riscv64-cosim CROSS_COMPILE=riscv64-linux-gnu-
```
