/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#ifndef WASI_WEBGPU_PRIVATE_H
#define WASI_WEBGPU_PRIVATE_H

#include "wasi_webgpu_types.h"
#include "wasm_export.h"

typedef struct {
    backend_type backend;
    void *backend_ctx;
} WASIWEBGPUContext;

#endif
