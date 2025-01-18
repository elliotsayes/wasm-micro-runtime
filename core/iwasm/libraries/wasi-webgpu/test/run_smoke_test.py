#!/usr/bin/env python3
#
# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#

from dataclasses import dataclass
from pathlib import Path
import subprocess
from typing import List


def execute_once(
    runtime_bin: str,
    runtime_args: List[str],
    wasm_file: str,
    wasm_args: List[str],
    cwd: Path,
) -> str:
    cmd = [runtime_bin]
    cmd.extend(runtime_args)
    cmd.append(wasm_file)
    cmd.extend(wasm_args)

    p = subprocess.run(
        cmd,
        cwd=cwd,
        capture_output=True,
        check=True,
        text=True,
        universal_newlines=True,
    )
    return p.stdout


def execute_triangle_example_once(
    runtime_bin: str, runtime_args: List[str], cwd: Path
) -> str:
    """
    Execute a basic WebGPU triangle example
    """
    wasm_file = "./triangle/target/wasm32-wasip1/debug/triangle.wasm"
    wasm_args = []
    return execute_once(runtime_bin, runtime_args, wasm_file, wasm_args, cwd)


def execute_compute_example_once(
    runtime_bin: str, runtime_args: List[str], cwd: Path
) -> str:
    """
    Execute a basic WebGPU compute shader example
    """
    wasm_file = "./compute/target/wasm32-wasip1/debug/compute.wasm"
    wasm_args = []
    return execute_once(runtime_bin, runtime_args, wasm_file, wasm_args, cwd)


def execute_triangle_example(iwasm_bin: str, wasmedge_bin: str, cwd: Path):
    """
    Run triangle example with both iwasm and wasmedge
    """
    print("\nExecuting WebGPU Triangle Example")
    print("=================================")

    iwasm_output = execute_triangle_example_once(
        iwasm_bin,
        ["--dir=.", "--env=RUST_LOG=debug"],
        cwd,
    )
    print("\nIWASM Output:")
    print(iwasm_output)

    wasmedge_output = execute_triangle_example_once(
        wasmedge_bin,
        ["--dir=.", "--env=RUST_LOG=debug"],
        cwd,
    )
    print("\nWASMEDGE Output:")
    print(wasmedge_output)


def execute_compute_example(iwasm_bin: str, wasmedge_bin: str, cwd: Path):
    """
    Run compute shader example with both iwasm and wasmedge
    """
    print("\nExecuting WebGPU Compute Example")
    print("================================")

    iwasm_output = execute_compute_example_once(
        iwasm_bin,
        ["--dir=.", "--env=RUST_LOG=debug"],
        cwd,
    )
    print("\nIWASM Output:")
    print(iwasm_output)

    wasmedge_output = execute_compute_example_once(
        wasmedge_bin,
        ["--dir=.", "--env=RUST_LOG=debug"],
        cwd,
    )
    print("\nWASMEDGE Output:")
    print(wasmedge_output)


def execute_webgpu_examples(iwasm_bin: str, wasmedge_bin: str):
    """
    Run all WebGPU examples
    """
    cwd = Path(__file__).parent

    execute_triangle_example(iwasm_bin, wasmedge_bin, cwd)
    execute_compute_example(iwasm_bin, wasmedge_bin, cwd)


if __name__ == "__main__":
    execute_webgpu_examples("iwasm", "wasmedge")
