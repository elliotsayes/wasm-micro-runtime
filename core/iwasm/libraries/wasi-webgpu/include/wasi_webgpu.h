/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#ifndef WASI_WEBGPU_H
#define WASI_WEBGPU_H

#include <stdint.h>
#include <stdbool.h>
#include "wasm_export.h"
#include "wasi_webgpu_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Functions */
int32_t device_create(wasm_exec_env_t exec_env, const adapter_options *options, device_handle *device);
int32_t buffer_create(wasm_exec_env_t exec_env, device_handle device, const buffer_descriptor *desc, buffer_handle *buffer);

/* Native symbol registration */
uint32_t get_native_symbol_count();
struct NativeSymbol *get_native_symbols();
uint32_t get_wasi_webgpu_export_apis(struct NativeSymbol **p_wasi_webgpu_apis);

#ifdef __cplusplus
}
#endif

#endif /* WASI_WEBGPU_H */
