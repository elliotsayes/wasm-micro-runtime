/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "bh_platform.h"
#include "wasm_export.h"
#include "wasi_webgpu_types.h"
#include "wasi_webgpu.h"
#include "wasi_webgpu_rust.h"

/* Context management */
typedef struct {
    uint32_t next_id;
} wasi_webgpu_ctx_t;

static wasi_webgpu_ctx_t *
wasi_webgpu_get_ctx(wasm_module_inst_t module_inst)
{
    wasi_webgpu_ctx_t *ctx = wasm_runtime_get_custom_data(module_inst);
    if (!ctx) {
        ctx = wasm_runtime_malloc(sizeof(wasi_webgpu_ctx_t));
        if (!ctx)
            return NULL;
        memset(ctx, 0, sizeof(wasi_webgpu_ctx_t));
        ctx->next_id = 1;
        wasm_runtime_set_custom_data(module_inst, ctx);
    }
    return ctx;
}

static void
wasi_webgpu_destroy_ctx(wasm_module_inst_t module_inst)
{
    wasi_webgpu_ctx_t *ctx = wasm_runtime_get_custom_data(module_inst);
    if (ctx) {
        wasm_runtime_free(ctx);
    }
}

int32_t
instance_request_adapter(wasm_exec_env_t exec_env, adapter_handle *adapter)
{
    wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env);
    wasi_webgpu_ctx_t *ctx = wasi_webgpu_get_ctx(module_inst);
    if (!ctx)
        return -1;

    adapter_handle handle = instance_request_adapter_rust();
    if (!handle)
        return -1;

    *adapter = handle;
    return 0;
}

int32_t
adapter_request_device(wasm_exec_env_t exec_env, adapter_handle adapter, device_handle *device)
{
    wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env);
    wasi_webgpu_ctx_t *ctx = wasi_webgpu_get_ctx(module_inst);
    if (!ctx)
        return -1;

    device_handle handle = adapter_request_device_rust(adapter);
    if (!handle)
        return -1;

    *device = handle;
    return 0;
}

int32_t
adapter_enumerate_features(wasm_exec_env_t exec_env, adapter_handle adapter, uint32_t *features, uint32_t *count)
{
    wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env);
    wasi_webgpu_ctx_t *ctx = wasi_webgpu_get_ctx(module_inst);
    if (!ctx)
        return -1;

    adapter_enumerate_features_rust(adapter, features, count);
    return 0;
}

int32_t
device_create(wasm_exec_env_t exec_env, const adapter_options *options, device_handle *device)
{
    wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env);
    wasi_webgpu_ctx_t *ctx = wasi_webgpu_get_ctx(module_inst);
    if (!ctx)
        return -1;

    // Call Rust FFI function
    device_handle handle = device_create_rust(options);
    if (!handle)
        return -1;

    *device = handle;
    return 0;
}

int32_t
buffer_create(wasm_exec_env_t exec_env, device_handle device, const buffer_descriptor *desc, buffer_handle *buffer)
{
    wasm_module_inst_t module_inst = wasm_runtime_get_module_inst(exec_env);
    wasi_webgpu_ctx_t *ctx = wasi_webgpu_get_ctx(module_inst);
    if (!ctx)
        return -1;

    // Call Rust FFI function
    buffer_handle handle = buffer_create_rust(device, desc);
    if (!handle)
        return -1;

    *buffer = handle;
    return 0;
}

/* Native symbol registration */
#define REG_NATIVE_FUNC(func_name, signature) \
    { "wasi_webgpu", #func_name, signature, NULL }

static NativeSymbol native_symbols[] = {
    REG_NATIVE_FUNC(instance_request_adapter, "(*)i"),
    REG_NATIVE_FUNC(adapter_request_device, "(i*)i"), 
    REG_NATIVE_FUNC(adapter_enumerate_features, "(i**)i"),
    REG_NATIVE_FUNC(device_create, "(i*)i"),
};

uint32_t
get_native_symbol_count()
{
    return sizeof(native_symbols) / sizeof(native_symbols[0]);
}

NativeSymbol *
get_native_symbols()
{
    return native_symbols;
}

/* Export APIs for WAMR */
uint32_t
get_wasi_webgpu_export_apis(NativeSymbol **p_wasi_webgpu_apis)
{
    *p_wasi_webgpu_apis = native_symbols;
    return get_native_symbol_count();
}
