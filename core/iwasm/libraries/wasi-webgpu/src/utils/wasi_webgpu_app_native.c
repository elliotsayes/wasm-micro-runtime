/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../wasi_webgpu_private.h"
#include "wasm_export.h"

// Forward declarations
static void convert_buffer_array_to_native(wasm_module_inst_t instance,
                                         gpu_buffer_array *builder_array,
                                         gpu_buffer_array *wasm_array);

static void convert_buffer_array_to_wasm(wasm_module_inst_t instance,
                                       gpu_buffer_array *wasm_array,
                                       gpu_buffer_array *native_array);

static void free_native_buffer_array(gpu_buffer_array *native_array);
static void free_wasm_buffer_array(gpu_buffer_array *wasm_array);

// WASM buffer structure
typedef struct {
    uint64_t buf_offset;
    uint32_t size;
} gpu_buffer_wasm;

// Convert buffer array from WASM to native
wasi_webgpu_error
gpu_buffer_array_app_native(wasm_module_inst_t instance,
                           gpu_buffer_array *builder_array,
                           uint64_t buf_offset, uint32_t size)
{
    if (!builder_array || !buf_offset || size == 0)
        return invalid_argument;

    gpu_buffer_wasm *wasm_buffers =
        (gpu_buffer_wasm *)wasm_runtime_addr_app_to_native(instance, buf_offset);

    // Create temporary WASM array
    gpu_buffer_array wasm_array;
    wasm_array.size = size;
    wasm_array.buf = (gpu_buffer_data *)wasm_runtime_malloc(size * sizeof(gpu_buffer_data));
    if (!wasm_array.buf)
        return out_of_memory;

    // Convert WASM buffer data
    for (uint32_t i = 0; i < size; i++) {
        gpu_buffer_data *wasm_buf = &wasm_array.buf[i];
        gpu_buffer_wasm *wasm_src = &wasm_buffers[i];

        wasm_buf->size = wasm_src->size;
        wasm_buf->data = (uint8_t *)wasm_src->buf_offset;
    }

    // Convert to native array
    convert_buffer_array_to_native(instance, builder_array, &wasm_array);

    // Cleanup temporary array
    wasm_runtime_free(wasm_array.buf);

    return success;
}

// Convert buffer array from WASM to native
static void
convert_buffer_array_to_native(wasm_module_inst_t instance,
                             gpu_buffer_array *builder_array,
                             gpu_buffer_array *wasm_array)
{
    if (!builder_array || !wasm_array)
        return;

    uint32_t size = wasm_array->size;
    if (size == 0)
        return;

    // Allocate memory for buffer array
    builder_array->buf = (gpu_buffer_data *)wasm_runtime_malloc(size * sizeof(gpu_buffer_data));
    if (!builder_array->buf)
        return;

    builder_array->size = size;

    for (uint32_t i = 0; i < size; i++) {
        gpu_buffer_data *native_buf = &builder_array->buf[i];
        gpu_buffer_data *wasm_buf = &wasm_array->buf[i];

        // Copy buffer data
        native_buf->size = wasm_buf->size;
        if (wasm_buf->size > 0) {
            native_buf->data = (uint8_t *)wasm_runtime_malloc(wasm_buf->size);
            if (!native_buf->data)
                continue;

            // Convert WASM offset to native pointer and copy data
            void *native_ptr = wasm_runtime_addr_app_to_native(instance, (uint64_t)wasm_buf->data);
            if (native_ptr) {
                memcpy(native_buf->data, native_ptr, wasm_buf->size);
            }
        }
    }
}

// Convert buffer array from native to WASM
static void
convert_buffer_array_to_wasm(wasm_module_inst_t instance,
                           gpu_buffer_array *wasm_array,
                           gpu_buffer_array *native_array)
{
    if (!wasm_array || !native_array)
        return;

    uint32_t size = native_array->size;
    if (size == 0)
        return;

    // Allocate memory for buffer array
    wasm_array->buf = (gpu_buffer_data *)wasm_runtime_malloc(size * sizeof(gpu_buffer_data));
    if (!wasm_array->buf)
        return;

    wasm_array->size = size;

    for (uint32_t i = 0; i < size; i++) {
        gpu_buffer_data *wasm_buf = &wasm_array->buf[i];
        gpu_buffer_data *native_buf = &native_array->buf[i];

        // Copy buffer data
        wasm_buf->size = native_buf->size;
        if (native_buf->size > 0) {
            // Allocate WASM memory and get offset
            uint64_t wasm_offset = wasm_runtime_module_malloc(instance, native_buf->size, (void **)&wasm_buf->data);
            if (wasm_offset == 0)
                continue;

            // Copy data to WASM memory
            void *native_ptr = wasm_runtime_addr_app_to_native(instance, wasm_offset);
            if (native_ptr) {
                memcpy(native_ptr, native_buf->data, native_buf->size);
            }
        }
    }
}

// Clean up native buffer array
static void
free_native_buffer_array(gpu_buffer_array *native_array)
{
    if (!native_array)
        return;

    if (native_array->buf) {
        for (uint32_t i = 0; i < native_array->size; i++) {
            if (native_array->buf[i].data)
                wasm_runtime_free(native_array->buf[i].data);
        }
        wasm_runtime_free(native_array->buf);
    }
}

// Clean up WASM buffer array
static void
free_wasm_buffer_array(gpu_buffer_array *wasm_array)
{
    if (!wasm_array)
        return;

    if (wasm_array->buf) {
        for (uint32_t i = 0; i < wasm_array->size; i++) {
            if (wasm_array->buf[i].data)
                wasm_runtime_free(wasm_array->buf[i].data);
        }
        wasm_runtime_free(wasm_array->buf);
    }
}

/* Native backend functions */
wasi_webgpu_error
wasi_webgpu_native_init(void **ctx)
{
    /* TODO: Initialize wgpu-native context */
    return success;
}

wasi_webgpu_error
wasi_webgpu_native_deinit(void *ctx)
{
    /* TODO: Clean up wgpu-native context */
    return success;
}

wasi_webgpu_error
wasi_webgpu_native_get_gpu(void *ctx, void **gpu)
{
    /* TODO: Get wgpu-native instance */
    return success;
}

wasi_webgpu_error
wasi_webgpu_native_request_adapter(void *ctx, void *gpu,
                                gpu_request_adapter_options *options,
                                void **adapter)
{
    /* TODO: Request wgpu-native adapter */
    return success;
}

wasi_webgpu_error
wasi_webgpu_native_request_device(void *ctx, void *adapter, void **device)
{
    /* TODO: Request wgpu-native device */
    return success;
}

wasi_webgpu_error
wasi_webgpu_native_device_create_buffer(void *ctx, void *device,
                                     gpu_buffer_descriptor *desc,
                                     void **buffer)
{
    /* TODO: Create wgpu-native buffer */
    return success;
}

wasi_webgpu_error
wasi_webgpu_native_device_create_texture(void *ctx, void *device,
                                      gpu_texture_descriptor *desc,
                                      void **texture)
{
    /* TODO: Create wgpu-native texture */
    return success;
}

wasi_webgpu_error
wasi_webgpu_native_device_create_shader_module(void *ctx, void *device,
                                           gpu_shader_module_descriptor *desc,
                                           void **shader)
{
    /* TODO: Create wgpu-native shader module */
    return success;
}

wasi_webgpu_error
wasi_webgpu_native_device_create_command_encoder(void *ctx, void *device,
                                             const char *label,
                                             void **encoder)
{
    /* TODO: Create wgpu-native command encoder */
    return success;
}

wasi_webgpu_error
wasi_webgpu_native_device_create_render_bundle_encoder(void *ctx, void *device,
                                                   gpu_render_bundle_encoder_descriptor *desc,
                                                   void **encoder)
{
    /* TODO: Create wgpu-native render bundle encoder */
    return success;
}
