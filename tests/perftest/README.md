# PerfTest

后端性能测试集合。每个测试是一个独立的 C 文件，放在 `tests/` 目录下。

`tests/backend-ipc.c` 的热点主体由独立整数 ALU 操作组成，使用 RVC 指令提高
每个 16 字节 fetch block 的有效指令数；热点主体没有分支，每 1792 条 ALU
操作只有一次循环回边。

使用 cosim 运行时关闭参考模型和波形，并打开性能计数器：

```sh
AM_HOME=/path/to/nexus-am make -C tests/perftest ARCH=riscv64-cosim \
  COSIM_REF= COSIM_COUNTERS=1 run
```

只运行单个测试时指定 `ALL`：

```sh
AM_HOME=/path/to/nexus-am make -C tests/perftest ARCH=riscv64-cosim \
  ALL=backend-ipc COSIM_REF= COSIM_COUNTERS=1 run
```

总 IPC 为 `retired / edges`。`[counter] Retired=... samples=...` 中
`value / samples` 是后端实际有指令可提交时的平均提交宽度，可用来区分后端
执行能力和前端供指能力。
