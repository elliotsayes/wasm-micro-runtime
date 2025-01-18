/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#ifndef WASI_WEBGPU_TYPES_H
#define WASI_WEBGPU_TYPES_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Error codes */
typedef enum {
    WASI_WEBGPU_SUCCESS = 0,
    WASI_WEBGPU_ERROR_INVALID_ARGUMENT,
    WASI_WEBGPU_ERROR_OUT_OF_MEMORY,
    WASI_WEBGPU_ERROR_DEVICE_LOST,
    WASI_WEBGPU_ERROR_VALIDATION_FAILED,
    WASI_WEBGPU_ERROR_UNSUPPORTED,
} wasi_webgpu_error;

/* Handle types */
typedef uint32_t device_handle;
typedef uint32_t buffer_handle;
typedef uint32_t texture_handle;
typedef uint32_t shader_handle;
typedef uint32_t pipeline_handle;
typedef uint32_t command_encoder_handle;
typedef uint32_t adapter_handle;

/* Power preference */
typedef enum {
    WASI_WEBGPU_POWER_PREFERENCE_UNDEFINED = 0,
    WASI_WEBGPU_POWER_PREFERENCE_LOW_POWER,
    WASI_WEBGPU_POWER_PREFERENCE_HIGH_PERFORMANCE,
} power_preference;

/* Adapter options */
typedef struct {
    power_preference power_preference;
    bool force_fallback_adapter;
} adapter_options;

/* Buffer usage flags */
typedef enum {
    WASI_WEBGPU_BUFFER_USAGE_MAP_READ = 1,
    WASI_WEBGPU_BUFFER_USAGE_MAP_WRITE = 2,
    WASI_WEBGPU_BUFFER_USAGE_COPY_SRC = 4,
    WASI_WEBGPU_BUFFER_USAGE_COPY_DST = 8,
    WASI_WEBGPU_BUFFER_USAGE_INDEX = 16,
    WASI_WEBGPU_BUFFER_USAGE_VERTEX = 32,
    WASI_WEBGPU_BUFFER_USAGE_UNIFORM = 64,
    WASI_WEBGPU_BUFFER_USAGE_STORAGE = 128,
    WASI_WEBGPU_BUFFER_USAGE_INDIRECT = 256,
} buffer_usage_flags;

/* Buffer descriptor */
typedef struct {
    const char *label;
    size_t size;
    buffer_usage_flags usage;
    bool mapped_at_creation;
} buffer_descriptor;

/* Vertex format */
typedef enum {
    WASI_WEBGPU_VERTEX_FORMAT_UINT8X2 = 0,
    WASI_WEBGPU_VERTEX_FORMAT_UINT8X4,
    WASI_WEBGPU_VERTEX_FORMAT_SINT8X2,
    WASI_WEBGPU_VERTEX_FORMAT_SINT8X4,
    WASI_WEBGPU_VERTEX_FORMAT_FLOAT32X2,
    WASI_WEBGPU_VERTEX_FORMAT_FLOAT32X3,
    WASI_WEBGPU_VERTEX_FORMAT_FLOAT32X4,
} vertex_format;

/* Vertex attribute */
typedef struct {
    vertex_format format;
    uint64_t offset;
    uint32_t shader_location;
} vertex_attribute;

/* Vertex buffer layout */
typedef struct {
    uint64_t array_stride;
    uint32_t attribute_count;
    const vertex_attribute *attributes;
    bool step_mode_vertex;
} vertex_buffer_layout;

/* Pipeline layout descriptor */
typedef struct {
    const char *label;
    uint32_t bind_group_layout_count;
    const uint32_t *bind_group_layouts;
} pipeline_layout_descriptor;

/* Render pipeline descriptor */
typedef struct {
    const char *label;
    pipeline_layout_descriptor layout;
    shader_handle vertex_shader;
    shader_handle fragment_shader;
    uint32_t vertex_buffer_count;
    const vertex_buffer_layout *vertex_buffers;
} render_pipeline_descriptor;

#ifdef __cplusplus
}
#endif

#endif /* WASI_WEBGPU_TYPES_H */
