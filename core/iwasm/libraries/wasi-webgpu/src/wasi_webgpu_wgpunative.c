/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */
#include "wasi_webgpu_types.h"
#include "utils/logger.h"
#include "wgpu.h"

// build info
extern int WGPU_BUILD_NUMBER;
extern char const *WGPU_COMMIT;
extern char const *WGPU_COMPILER;
extern char const *WGPU_BUILD_TARGET;

struct wasi_webgpu_wgpunative_config {
    // Backend(plugin in WasmEdge) parameters:
    bool enable_log;
    bool enable_debug_log;
    
    // TODO
};

__attribute__((visibility("default"))) wasi_webgpu_error
init_backend(void **ctx)
{
    // TODO
}

__attribute__((visibility("default"))) wasi_webgpu_error
deinit_backend(void *ctx)
{
    // TODO
}

// TODO
