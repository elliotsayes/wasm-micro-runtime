/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#ifndef WASI_WEBGPU_APP_NATIVE_H
#define WASI_WEBGPU_APP_NATIVE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bh_platform.h"
#include "wasi_webgpu_types.h"

#ifdef __cplusplus
extern "C" {
#endif

wasi_webgpu_error
wasi_webgpu_native_init(void **ctx);

wasi_webgpu_error
wasi_webgpu_native_deinit(void *ctx);

wasi_webgpu_error
wasi_webgpu_native_get_gpu(void *ctx, void **gpu);

wasi_webgpu_error
wasi_webgpu_native_request_adapter(void *ctx, void *gpu,
                                 gpu_request_adapter_options *options,
                                 void **adapter);

wasi_webgpu_error
wasi_webgpu_native_request_device(void *ctx, void *adapter, void **device);

wasi_webgpu_error
wasi_webgpu_native_device_create_buffer(void *ctx, void *device,
                                      gpu_buffer_descriptor *desc,
                                      void **buffer);

wasi_webgpu_error
wasi_webgpu_native_device_create_texture(void *ctx, void *device,
                                       gpu_texture_descriptor *desc,
                                       void **texture);

wasi_webgpu_error
wasi_webgpu_native_device_create_shader_module(void *ctx, void *device,
                                            gpu_shader_module_descriptor *desc,
                                            void **shader);

wasi_webgpu_error
wasi_webgpu_native_device_create_command_encoder(void *ctx, void *device,
                                              const char *label,
                                              void **encoder);

wasi_webgpu_error
wasi_webgpu_native_device_create_render_bundle_encoder(void *ctx, void *device,
                                                    gpu_render_bundle_encoder_descriptor *desc,
                                                    void **encoder);

#ifdef __cplusplus
}
#endif

#endif /* WASI_WEBGPU_APP_NATIVE_H */
