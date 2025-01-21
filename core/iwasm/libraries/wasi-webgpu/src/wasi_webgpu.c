/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <dlfcn.h>

#include "wasi_webgpu_private.h"
#include "utils/wasi_webgpu_app_native.h"
#include "utils/logger.h"

#include "bh_platform.h"
#include "wasi_webgpu_types.h"
#include "wasm_export.h"

#define HASHMAP_INITIAL_SIZE 20
#define WGPUNATIVE_BACKEND_LIB_PREFIX "libwasi_webgpu_wgpunative"

/* Global variables */
struct backends_api_functions {
    void *backend_handle;
    api_function functions;
} lookup[autodetect + 1] = { 0 };

#define call_wasi_webgpu_func(backend_encoding, func, wasi_error, ...)         \
    do {                                                                   \
        wasi_error = lookup[backend_encoding].functions.func(__VA_ARGS__); \
        if (wasi_error != success)                                         \
            NN_ERR_PRINTF("Error %s() -> %d", #func, wasi_error);          \
    } while (0)

/* HashMap utils */
static HashMap *hashmap;

static uint32
hash_func(const void *key)
{
    // fnv1a_hash
    const uint32 FNV_PRIME = 16777619;
    const uint32 FNV_OFFSET_BASIS = 2166136261U;

    uint32 hash = FNV_OFFSET_BASIS;
    const unsigned char *bytes = (const unsigned char *)key;

    for (size_t i = 0; i < sizeof(uintptr_t); ++i) {
        hash ^= bytes[i];
        hash *= FNV_PRIME;
    }

    return hash;
}

static bool
key_equal_func(void *key1, void *key2)
{
    return key1 == key2;
}

static void
key_destroy_func(void *key1)
{
    /* key type is wasm_module_inst_t*. do nothing */
}

/* Get filename (with extension) of a given library name based on OS */
static bool
get_backend_lib_filename(char *lib_name, char *filename)
{
#if defined(_WIN32)
    // Windows uses .dll
    const char *extension = "dll";
#elif defined(__APPLE__)
    // macOS uses .dylib
    const char *extension = "dylib";
#else
    // Linux uses .so
    const char *extension = "so";
#endif

    if (!lib_name || !filename)
        return false;

    snprintf(filename, strlen(lib_name) + strlen(extension) + 1, 
             "%s.%s", lib_name, extension);
    return true;
}

static void
wasi_webgpu_ctx_destroy(WASIWEBGPUContext *wasi_webgpu_ctx)
{
    NN_DBG_PRINTF("[WASI WEBGPU] DEINIT...");

    if (wasi_webgpu_ctx == NULL) {
        NN_ERR_PRINTF(
            "Error when deallocating memory. WASI-WEBGPU context is NULL");
        return;
    }
    NN_DBG_PRINTF("Freeing wasi-webgpu");
    NN_DBG_PRINTF("-> current_encoding: %d", wasi_webgpu_ctx->backend);

    /* deinit() the backend */
    wasi_webgpu_error res;
    call_wasi_webgpu_func(wasi_webgpu_ctx->backend, deinit, res,
                      wasi_webgpu_ctx->backend_ctx);

    wasm_runtime_free(wasi_webgpu_ctx);
}

static void
value_destroy_func(void *value)
{
    wasi_webgpu_ctx_destroy((WASIWEBGPUContext *)value);
}

bool
wasi_webgpu_initialize()
{
    NN_DBG_PRINTF("[WASI WEBGPU General] Initializing wasi-webgpu");

    // hashmap { instance: wasi_webgpu_ctx }
    hashmap = bh_hash_map_create(HASHMAP_INITIAL_SIZE, true, hash_func,
                                 key_equal_func, key_destroy_func,
                                 value_destroy_func);
    if (hashmap == NULL) {
        NN_ERR_PRINTF("Error while initializing hashmap");
        return false;
    }

    return true;
}

static WASIWEBGPUContext *
wasi_webgpu_initialize_context()
{
    NN_DBG_PRINTF("[WASI WEBGPU] INIT...");

    WASIWEBGPUContext *wasi_webgpu_ctx =
        (WASIWEBGPUContext *)wasm_runtime_malloc(sizeof(WASIWEBGPUContext));
    if (wasi_webgpu_ctx == NULL) {
        NN_ERR_PRINTF("Error when allocating memory for WASI-WEBGPU context");
        return NULL;
    }

    memset(wasi_webgpu_ctx, 0, sizeof(WASIWEBGPUContext));
    return wasi_webgpu_ctx;
}

/* Get wasi-webgpu context from module instance */
static WASIWEBGPUContext *
wasm_runtime_get_wasi_webgpu_ctx(wasm_module_inst_t instance)
{
    WASIWEBGPUContext *wasi_webgpu_ctx =
        (WASIWEBGPUContext *)bh_hash_map_find(hashmap, (void *)instance);
    if (wasi_webgpu_ctx == NULL) {
        wasi_webgpu_ctx = wasi_webgpu_initialize_context();
        if (wasi_webgpu_ctx == NULL)
            return NULL;

        bool ok =
            bh_hash_map_insert(hashmap, (void *)instance, (void *)wasi_webgpu_ctx);
        if (!ok) {
            NN_ERR_PRINTF("Error while storing context");
            wasi_webgpu_ctx_destroy(wasi_webgpu_ctx);
            return NULL;
        }
    }

    return wasi_webgpu_ctx;
}

void
wasi_webgpu_destroy()
{
    // destroy hashmap will destroy keys and values
    bh_hash_map_destroy(hashmap);

    // close backends' libraries and registered functions
    for (unsigned i = 0; i < sizeof(lookup) / sizeof(lookup[0]); i++) {
        if (lookup[i].backend_handle) {
            dlclose(lookup[i].backend_handle);
            lookup[i].backend_handle = NULL;
        }

        memset(&lookup[i].functions, 0, sizeof(api_function));
    }
}

static bool
register_backend(void *handle, api_function *functions)
{
    BACKEND_INITIALIZE init = (BACKEND_INITIALIZE)dlsym(handle, "init_backend");
    if (!init) {
        NN_WARN_PRINTF("init_backend() not found");
        return false;
    }
    functions->init = init;

    BACKEND_DEINITIALIZE deinit =
        (BACKEND_DEINITIALIZE)dlsym(handle, "deinit_backend");
    if (!deinit) {
        NN_WARN_PRINTF("deinit_backend() not found");
        return false;
    }
    functions->deinit = deinit;

    // TODO: WebGPU functions

    return true;
}

static bool
prepare_backend(const char *lib_name, struct backends_api_functions *backend)
{
    NN_DBG_PRINTF("[Native Register] prepare_backend %s", lib_name);

    void *handle;
    handle = dlopen(lib_name, RTLD_LAZY);
    if (!handle) {
        NN_ERR_PRINTF("Error loading %s. %s", lib_name, dlerror());
        return false;
    }

    if (!register_backend(handle, &(backend->functions))) {
        NN_ERR_PRINTF("Error when registering functions of %s", lib_name);
        dlclose(handle);
        return false;
    }

    backend->backend_handle = handle;
    return true;
}

static bool
detect_and_load_backend(struct backends_api_functions *backends)
{
    char backend_lib_name[128];
    get_backend_lib_filename(WGPUNATIVE_BACKEND_LIB_PREFIX, backend_lib_name);

    return prepare_backend(backend_lib_name, backends);
}

/* WASI-WEBGPU implementation */

#if WASM_ENABLE_WASI_WEBGPU != 0

wasi_webgpu_error
wasi_webgpu_get_gpu(wasm_exec_env_t exec_env, void **gpu)
{
    wasi_webgpu_error err = success;
    WASIWEBGPUContext *wasi_webgpu_ctx = wasm_runtime_get_wasi_webgpu_ctx(
        wasm_runtime_get_module_inst(exec_env));

    if (!wasi_webgpu_ctx || !gpu)
        return invalid_argument;

    call_wasi_webgpu_func(wasi_webgpu_ctx->backend, get_gpu,
                         err, wasi_webgpu_ctx->backend_ctx, gpu);
    return err;
}

wasi_webgpu_error
wasi_webgpu_request_adapter(wasm_exec_env_t exec_env, void *gpu, 
                           gpu_request_adapter_options *options, void **adapter)
{
    wasi_webgpu_error err = success;
    WASIWEBGPUContext *wasi_webgpu_ctx = wasm_runtime_get_wasi_webgpu_ctx(
        wasm_runtime_get_module_inst(exec_env));

    if (!wasi_webgpu_ctx || !gpu || !adapter)
        return invalid_argument;

    call_wasi_webgpu_func(wasi_webgpu_ctx->backend, request_adapter,
                         err, wasi_webgpu_ctx->backend_ctx, gpu, options, adapter);
    return err;
}

wasi_webgpu_error
wasi_webgpu_request_device(wasm_exec_env_t exec_env, void *adapter, void **device)
{
    wasi_webgpu_error err = success;
    WASIWEBGPUContext *wasi_webgpu_ctx = wasm_runtime_get_wasi_webgpu_ctx(
        wasm_runtime_get_module_inst(exec_env));

    if (!wasi_webgpu_ctx || !adapter || !device)
        return invalid_argument;

    call_wasi_webgpu_func(wasi_webgpu_ctx->backend, request_device,
                         err, wasi_webgpu_ctx->backend_ctx, adapter, device);
    return err;
}

wasi_webgpu_error
wasi_webgpu_device_create_buffer(wasm_exec_env_t exec_env, void *device,
                                gpu_buffer_descriptor *desc, void **buffer)
{
    wasi_webgpu_error err = success;
    WASIWEBGPUContext *wasi_webgpu_ctx = wasm_runtime_get_wasi_webgpu_ctx(
        wasm_runtime_get_module_inst(exec_env));

    if (!wasi_webgpu_ctx || !device || !desc || !buffer)
        return invalid_argument;

    call_wasi_webgpu_func(wasi_webgpu_ctx->backend, create_buffer,
                         err, wasi_webgpu_ctx->backend_ctx, device, desc, buffer);
    return err;
}

wasi_webgpu_error
wasi_webgpu_device_create_texture(wasm_exec_env_t exec_env, void *device,
                                 gpu_texture_descriptor *desc, void **texture)
{
    wasi_webgpu_error err = success;
    WASIWEBGPUContext *wasi_webgpu_ctx = wasm_runtime_get_wasi_webgpu_ctx(
        wasm_runtime_get_module_inst(exec_env));

    if (!wasi_webgpu_ctx || !device || !desc || !texture)
        return invalid_argument;

    call_wasi_webgpu_func(wasi_webgpu_ctx->backend, create_texture,
                         err, wasi_webgpu_ctx->backend_ctx, device, desc, texture);
    return err;
}

wasi_webgpu_error
wasi_webgpu_device_create_shader_module(wasm_exec_env_t exec_env, void *device,
                                      gpu_shader_module_descriptor *desc, void **shader)
{
    wasi_webgpu_error err = success;
    WASIWEBGPUContext *wasi_webgpu_ctx = wasm_runtime_get_wasi_webgpu_ctx(
        wasm_runtime_get_module_inst(exec_env));

    if (!wasi_webgpu_ctx || !device || !desc || !shader)
        return invalid_argument;

    call_wasi_webgpu_func(wasi_webgpu_ctx->backend, create_shader_module,
                         err, wasi_webgpu_ctx->backend_ctx, device, desc, shader);
    return err;
}

wasi_webgpu_error
wasi_webgpu_device_create_command_encoder(wasm_exec_env_t exec_env, void *device,
                                        const char *label, void **encoder)
{
    wasi_webgpu_error err = success;
    WASIWEBGPUContext *wasi_webgpu_ctx = wasm_runtime_get_wasi_webgpu_ctx(
        wasm_runtime_get_module_inst(exec_env));

    if (!wasi_webgpu_ctx || !device || !encoder)
        return invalid_argument;

    call_wasi_webgpu_func(wasi_webgpu_ctx->backend, create_command_encoder,
                         err, wasi_webgpu_ctx->backend_ctx, device, label, encoder);
    return err;
}

wasi_webgpu_error
wasi_webgpu_device_create_render_bundle_encoder(wasm_exec_env_t exec_env, void *device,
                                              gpu_render_bundle_encoder_descriptor *desc,
                                              void **encoder)
{
    wasi_webgpu_error err = success;
    WASIWEBGPUContext *wasi_webgpu_ctx = wasm_runtime_get_wasi_webgpu_ctx(
        wasm_runtime_get_module_inst(exec_env));

    if (!wasi_webgpu_ctx || !device || !desc || !encoder)
        return invalid_argument;

    call_wasi_webgpu_func(wasi_webgpu_ctx->backend, create_render_bundle_encoder,
                         err, wasi_webgpu_ctx->backend_ctx, device, desc, encoder);
    return err;
}

#endif /* WASM_ENABLE_WASI_WEBGPU != 0 */

/* Register native functions */
#define REG_NATIVE_FUNC(func_name, signature) \
    { #func_name, wasi_webgpu_##func_name, signature, NULL }

static NativeSymbol native_symbols[] = {
    REG_NATIVE_FUNC(get_gpu, "(**)i"),
    REG_NATIVE_FUNC(request_adapter, "(***)i"),
    REG_NATIVE_FUNC(request_device, "(***)i"),
    REG_NATIVE_FUNC(device_create_buffer, "(****)i"),
    REG_NATIVE_FUNC(device_create_texture, "(****)i"),
    REG_NATIVE_FUNC(device_create_shader_module, "(****)i"),
    REG_NATIVE_FUNC(device_create_command_encoder, "(***)i"),
    REG_NATIVE_FUNC(device_create_render_bundle_encoder, "(****)i"),
};

uint32_t
get_wasi_webgpu_export_apis(NativeSymbol **p_native_symbols)
{
    *p_native_symbols = native_symbols;
    return sizeof(native_symbols) / sizeof(NativeSymbol);
}
