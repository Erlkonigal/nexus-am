# PerfTest

后端性能测试集合。每个测试是一个独立的 C 文件，放在 `tests/` 目录下。

`tests/backend-ipc.c` 的热点主体由独立整数 ALU 操作组成，使用 RVC 指令提高
每个 fetch block 的有效指令数；每 112 条 ALU 操作有一次循环回边，利用分支
预测减少所需的循环展开。循环体仅约 228 字节，可减少冷启动取指和 I-cache
填充开销。循环执行 4096 次，总 ALU 操作数仍为 458752，校验值不变。

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
