/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

/**
 * Following definition from:
 * [Oct 25th, 2022]
 * https://github.com/WebAssembly/wasi-webgpu/blob/0f77c48ec195748990ff67928a4b3eef5f16c2de/wasi-webgpu.wit.md
 */

#ifndef WASI_WEBGPU_H
#define WASI_WEBGPU_H

#include <stdint.h>
#include "wasi_webgpu_types.h"

wasi_webgpu_error
do_something(uint32_t some_var)
    __attribute__((import_module("wasi_webgpu")));

#endif
