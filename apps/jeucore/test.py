#!/usr/bin/env python3
"""ELF and Verilator checks owned by the Jeucore AM application."""
import argparse
import re
import subprocess


def output(command):
    result = subprocess.run(command, capture_output=True, text=True, timeout=30)
    if result.returncode:
        raise AssertionError(result.stdout + result.stderr)
    return result.stdout


def check_elf(args):
    header = output([args.readelf, "-h", args.elf])
    assert "ELF64" in header and "RISC-V" in header
    assert re.search(r"Flags:\s+0x0\b", header) or "soft-float ABI" in header
    assert re.search(r'Tag_RISCV_arch:\s+"rv64i', output([args.readelf, "-A", args.elf]))
    assert re.search(rf"Entry point address:\s+0x{args.ram_base:x}\b", header)

    symbols = output([args.nm, "-n", args.elf])
    for name, address in (("_start", args.ram_base), ("_sim_pass", args.pass_pc),
                          ("_sim_fail", args.fail_pc)):
        assert re.search(rf"(?m)^{address:016x}\s+[Tt]\s+{name}$", symbols), name

    segments = output([args.readelf, "-lW", args.elf])
    loads = re.findall(r"(?m)^\s*LOAD\s+\S+\s+\S+\s+(0x[0-9a-f]+)\s+\S+\s+(0x[0-9a-f]+)", segments)
    assert loads, "ELF has no load segments"
    for address, size in loads:
        start, length = int(address, 16), int(size, 16)
        assert args.ram_base <= start and start + length <= args.ram_base + args.ram_size


def check_run(args, uart_input, success, expected):
    command = [args.sim, f"+elf={args.elf}", f"+pass-pc={args.pass_pc:#x}",
               f"+fail-pc={args.fail_pc:#x}", f"+uart-input={uart_input}",
               "+cosim=0", "+wave=0", "+counters=0", "+trace=none", "+max-cycles=100000"]
    result = subprocess.run(command, capture_output=True, text=True, timeout=30)
    assert (result.returncode == 0) == success, result.stdout + result.stderr
    assert result.stdout == expected, result.stdout + result.stderr
    marker = "[sim] PASS" if success else "software failure PC retired"
    assert marker in result.stderr, result.stderr


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--elf", required=True)
    parser.add_argument("--sim", required=True)
    parser.add_argument("--readelf", required=True)
    parser.add_argument("--nm", required=True)
    for name in ("ram-base", "ram-size", "pass-pc", "fail-pc"):
        parser.add_argument("--" + name, type=lambda value: int(value, 0), required=True)
    args = parser.parse_args()
    check_elf(args)
    check_run(args, "Z", True, "wave 42\nOK\n")
    check_run(args, "X", False, "FAIL\n")
    print("Jeucore AM checks passed")


if __name__ == "__main__":
    main()
