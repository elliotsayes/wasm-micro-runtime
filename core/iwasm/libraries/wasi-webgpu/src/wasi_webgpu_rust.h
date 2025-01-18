/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#ifndef WASI_WEBGPU_RUST_H
#define WASI_WEBGPU_RUST_H

#include <stdint.h>
#include <stddef.h>
#include "../include/wasi_webgpu_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Rust FFI functions */

/**
 * Device operations
 */
int32_t rust_webgpu_device_create(const adapter_options *options, 
                                 uint64_t *device_ptr);

void rust_webgpu_device_destroy(uint64_t device_ptr);

/**
 * Buffer operations
 */
int32_t rust_webgpu_buffer_create(uint64_t device_ptr,
                                 const buffer_descriptor *desc,
                                 uint64_t *buffer_ptr);

int32_t rust_webgpu_buffer_write(uint64_t buffer_ptr,
                                const uint8_t *data,
                                size_t size,
                                size_t offset);

void rust_webgpu_buffer_destroy(uint64_t buffer_ptr);

/**
 * Shader operations
 */
int32_t rust_webgpu_shader_create(uint64_t device_ptr,
                                 const char *source,
                                 uint64_t *shader_ptr);

void rust_webgpu_shader_destroy(uint64_t shader_ptr);

/**
 * Pipeline operations
 */
int32_t rust_webgpu_render_pipeline_create(uint64_t device_ptr,
                                         const render_pipeline_descriptor *desc,
                                         uint64_t *pipeline_ptr);

void rust_webgpu_pipeline_destroy(uint64_t pipeline_ptr);

/**
 * Command operations
 */
int32_t rust_webgpu_command_encoder_create(uint64_t device_ptr,
                                         uint64_t *encoder_ptr);

int32_t rust_webgpu_command_encoder_submit(uint64_t device_ptr,
                                         uint64_t encoder_ptr);

void rust_webgpu_command_encoder_destroy(uint64_t encoder_ptr);

#ifdef __cplusplus
}
#endif

#endif /* WASI_WEBGPU_RUST_H */
