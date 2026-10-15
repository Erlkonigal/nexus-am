# The Abstract Machine (AM)

## Jeucore 仿真平台

`ARCH=riscv64-jeucore` 使用 RV64I、LP64 ABI 和 Jeucore 的 RAM/UART 地址，生成可由
Jeucore Verilator 模拟器通过 `+elf=PATH` 加载的 ELF。该平台使用 `klib`，无需 newlib。

```sh
export AM_HOME="$PWD"
make -C apps/jeucore ARCH=riscv64-jeucore CROSS_COMPILE=riscv64-unknown-elf-
make -C apps/jeucore ARCH=riscv64-jeucore CROSS_COMPILE=riscv64-unknown-elf- \
  JEUCORE_SIM=/path/to/Jeucore/build/VSoC run
```

`run` 将 ELF 交给模拟器；`JEUCORE_SIM_ARGS` 可传递额外 plusargs。平台地址和 UART
参数可通过 `JEUCORE_RAM_BASE`、`JEUCORE_RAM_SIZE`、`JEUCORE_UART_BASE`、
`JEUCORE_UART_CLOCK_HZ`、`JEUCORE_UART_BAUD`、`JEUCORE_SIM_PASS_ADDR` 和
`JEUCORE_SIM_FAIL_ADDR` 覆盖。修改这些参数时，请使用 `make -B` 重编译，并让
Jeucore Kconfig 使用相同值。当前 RTL 只验证了默认 pass/fail 地址的运行行为。

在仓库根目录运行 `make bear` 可用 Bear 重编 `apps/jeucore` 及其依赖，并生成
`compile_commands.json`。默认架构为 `riscv64-jeucore`；可以指定重编目录、架构
和交叉编译器，例如：

```sh
make bear CROSS_COMPILE=riscv64-linux-gnu-
make bear BEAR_DIR=apps/coremark ARCH=riscv64-jeucore CROSS_COMPILE=riscv64-linux-gnu-
```

`BEAR_DIR` 也可以指向 `am` 或 `libs/klib` 等含 Makefile 的目录。

抽象计算机：必要的抽象，帮你编写一个完整的计算机系统！

目录组织：

* `am/`：AM头文件、每个体系结构分别实现的AM代码。
* `libs/`：建立在AM上、体系结构无关的运行库，如软件模拟浮点数、基础libc功能等。
* `apps/`：一些运行在AM上应用程序示例。
* `tests/`: 用来测试AM实现的简单测试程序。

## 创建一个AM应用

在任何目录都可以创建运行在AM上的应用程序。程序用C/C++语言编写，除AM之外无法调用其他库函数（但可以引用`stdarg.h`, `limits.h`等包含体系结构相关数据定义的头文件）。

为此你需要在应用程序项目的根目录添加一个Makefile：

```
NAME = app-name
SRCS = src/main.c src/help.c src/lib.cpp
include $(AM_HOME)/Makefile.app
```

一些注意事项：

* `NAME`定义了应用的名字。编译后会在`build/`目录里出现以此命名的应用程序。
* `SRCS`指定了编译应用所需的源文件。可以放在应用目录中的任意位置。

* 应用目录下的`include/`目录会被添加到编译的`-I`选项中。
* 环境变量`AM_HOME`需要包含**nexus-am项目的根目录的绝对路径**。

编译时，首先确保`AM_HOME`正确设置，然后执行`make ARCH=体系结构名`编译。例如`make ARCH=native`将会编译成本地可运行的项目，`make ARCH=mips32-minimal`生成用于仿真的MIPS32程序。`ARCH`缺省时默认编译到本地。
