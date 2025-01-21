/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#ifndef WASI_WEBGPU_APP_NATIVE
#define WASI_WEBGPU_APP_NATIVE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

#include "wasi_webgpu_types.h"
#include "logger.h"

#include "bh_platform.h"
#include "wasm_export.h"

typedef struct {
    uint32_t buf_offset;
    uint32_t size;
} mybuffer_wasm;

typedef struct {
    uint32_t buf_offset;
    uint32_t size;
} mybuffer_array_wasm;

wasi_webgpu_error
mybuffer_array_app_native(wasm_module_inst_t instance,
                          mybuffer_array_wasm *builder,
                          mybuffer_array *builder_native);

#endif
