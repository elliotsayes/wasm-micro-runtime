/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#ifndef WASI_WEBGPU_TYPES_H
#define WASI_WEBGPU_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// Basic types from WIT spec
typedef uint64_t gpu_size64;
typedef uint32_t gpu_size32;
typedef uint32_t gpu_flags_constant;
typedef uint32_t gpu_integer_coordinate;

// Forward declarations
struct gpu_queue;
struct gpu_compilation_info;
struct gpu_compilation_message;
struct gpu_render_bundle;
struct gpu_render_pass_encoder;
struct gpu_compute_pass_encoder;

typedef struct gpu_queue gpu_queue;
typedef struct gpu_compilation_info gpu_compilation_info;
typedef struct gpu_compilation_message gpu_compilation_message;
typedef struct gpu_render_bundle gpu_render_bundle;
typedef struct gpu_render_pass_encoder gpu_render_pass_encoder;
typedef struct gpu_compute_pass_encoder gpu_compute_pass_encoder;

// Forward declarations for opaque types
struct gpu_texture;
struct gpu_sampler;
struct gpu_bind_group_layout;
struct gpu_texture_view;
struct gpu_bind_group;
struct gpu_pipeline_layout;
struct gpu_shader_module;
struct gpu_compute_pipeline;
struct gpu_render_pipeline;
struct gpu_command_encoder;
struct gpu_render_bundle_encoder;
struct gpu_query_set;

typedef struct gpu_texture gpu_texture;
typedef struct gpu_sampler gpu_sampler;
typedef struct gpu_bind_group_layout gpu_bind_group_layout;
typedef struct gpu_texture_view gpu_texture_view;
typedef struct gpu_bind_group gpu_bind_group;
typedef struct gpu_pipeline_layout gpu_pipeline_layout;
typedef struct gpu_shader_module gpu_shader_module;
typedef struct gpu_compute_pipeline gpu_compute_pipeline;
typedef struct gpu_render_pipeline gpu_render_pipeline;
typedef struct gpu_command_encoder gpu_command_encoder;
typedef struct gpu_render_bundle_encoder gpu_render_bundle_encoder;
typedef struct gpu_query_set gpu_query_set;

// Basic buffer types
typedef struct {
    uint8_t *data;
    size_t size;
} gpu_buffer_data;

// GPU Buffer type
typedef struct gpu_buffer {
    gpu_buffer_data data;
    uint32_t usage;
    bool mapped_at_creation;
} gpu_buffer;

// Error types
typedef enum {
    gpu_error_validation,
    gpu_error_out_of_memory,
    gpu_error_internal,
    gpu_error_unknown,
} gpu_error;

typedef enum {
    gpu_texture_aspect_all,
    gpu_texture_aspect_stencil_only,
    gpu_texture_aspect_depth_only,
} gpu_texture_aspect;

typedef enum {
    success = 0,
    invalid_argument,
    invalid_operation,
    out_of_memory,
    device_lost,
    validation_error,
    unsupported_feature,
    unknown
} wasi_webgpu_error;

// Option types for error handling
typedef struct {
    bool has_value;
    union {
        gpu_error error;
        void *none;
    } value;
} gpu_error_option;

typedef enum {
    wgpunative,
    autodetect,
} backend_type;

// WebGPU specific types from webgpu.wit
typedef enum {
    low_power,
    high_performance,
} gpu_power_preference;

typedef enum {
    depth_clip_control,
    depth32float_stencil8,
    texture_compression_bc,
    texture_compression_bc_sliced3d,
    texture_compression_etc2,
    texture_compression_astc,
    timestamp_query,
    indirect_first_instance,
    shader_f16,
    rg11b10ufloat_renderable,
    bgra8unorm_storage,
    float32_filterable,
    clip_distances,
    dual_source_blending,
} gpu_feature_name;

typedef enum {
    r8unorm,
    r8snorm,
    r8uint,
    r8sint,
    r16uint,
    r16sint,
    r16float,
    rg8unorm,
    rg8snorm,
    rg16float,
    rgba8unorm,
    rgba8unorm_srgb,
    rgba8snorm,
    rgba16float,
    rgba32float,
} gpu_texture_format;

typedef enum {
    vertex = 1,
    fragment = 2,
    compute = 4,
} gpu_shader_stage;

typedef enum {
    undefined,
    load,
    store,
    clear,
    discard,
} gpu_store_op;

typedef enum {
    unmapped,
    pending,
    mapped,
} gpu_buffer_map_state;

typedef enum {
    map_read = 1,
    map_write = 2,
} gpu_map_mode_flags;

typedef enum {
    error_filter_error,
    error_filter_out_of_memory,
    error_filter_validation,
    error_filter_internal,
} gpu_error_filter;

// Texture types
typedef enum {
    d1,
    d2,
    d3,
} gpu_texture_dimension;

typedef enum {
    validation,
    internal,
    out_of_resources,
} gpu_pipeline_error_reason;

// Color and coordinate types
typedef struct {
    double r;
    double g;
    double b;
    double a;
} gpu_color;

typedef struct {
    uint32_t x;
    uint32_t y;
    uint32_t z;
} gpu_origin3_d;

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t depth_or_array_layers;
} gpu_extent3_d;

// Copy types
typedef struct {
    void *buffer;
    uint64_t offset;
    uint64_t bytes_per_row;
    uint32_t rows_per_image;
} gpu_image_copy_buffer;

typedef struct {
    void *texture;
    uint32_t mip_level;
    gpu_origin3_d origin;
    gpu_texture_aspect aspect;
} gpu_image_copy_texture;

// Stencil state
typedef struct {
    uint32_t compare;
    uint32_t fail_op;
    uint32_t depth_fail_op;
    uint32_t pass_op;
} gpu_stencil_face_state;

typedef struct {
    gpu_buffer_data *buf;
    uint32_t size;
} gpu_buffer_array;

// Bind group related types
typedef struct {
    uint32_t binding;
    uint32_t visibility;
    uint32_t buffer;
    uint32_t sampler;
    uint32_t texture;
    uint32_t storage_texture;
} gpu_bind_group_layout_entry;

typedef struct {
    uint32_t binding;
    const gpu_buffer *buffer;
    uint64_t offset;
    uint64_t size;
    const gpu_sampler *sampler;
    const gpu_texture_view *texture_view;
} gpu_bind_group_entry;

typedef struct {
    const char *label;
    const gpu_bind_group_layout_entry *entries;
    uint32_t entries_length;
} gpu_bind_group_layout_descriptor;

typedef struct {
    const char *label;
    const gpu_bind_group_layout *layout;
    const gpu_bind_group_entry *entries;
    uint32_t entries_length;
} gpu_bind_group_descriptor;

// Pipeline descriptors
typedef struct {
    const char *label;
    const char *code;
    const char *source_map;
    const char **defines;
    uint32_t defines_length;
} gpu_shader_module_descriptor;

typedef struct {
    const char *label;
    gpu_texture_format format;
    uint32_t sample_count;
    uint32_t usage;
    uint32_t view_dimension;
} gpu_render_bundle_encoder_descriptor;

typedef struct {
    const char *label;
    const gpu_bind_group_layout **layouts;
    uint32_t layouts_length;
} gpu_pipeline_layout_descriptor;

typedef struct {
    const char *label;
    gpu_shader_module_descriptor *compute;
    void *pipeline_layout;
} gpu_compute_pipeline_descriptor;

typedef struct {
    const char *label;
    gpu_shader_module_descriptor *vertex;
    gpu_shader_module_descriptor *fragment;
    void *pipeline_layout;
    uint32_t primitive_topology;
    uint32_t strip_index_format;
    uint32_t front_face;
    uint32_t cull_mode;
    bool unclipped_depth;
    uint32_t depth_bias;
    float depth_bias_slope_scale;
    float depth_bias_clamp;
    bool depth_write_enabled;
    uint32_t depth_compare;
    uint32_t stencil_front;
    uint32_t stencil_back;
    uint32_t vertex_state;
    uint32_t primitive_state;
    uint32_t depth_stencil_state;
    uint32_t multisample_state;
    uint32_t fragment_state;
} gpu_render_pipeline_descriptor;

typedef struct {
    const char *label;
    gpu_texture_format format;
    bool depth_write_enabled;
    uint32_t depth_compare;
    gpu_stencil_face_state stencil_front;
    gpu_stencil_face_state stencil_back;
    uint32_t stencil_read_mask;
    uint32_t stencil_write_mask;
    int32_t depth_bias;
    float depth_bias_slope_scale;
    float depth_bias_clamp;
} gpu_depth_stencil_state;

// Device limits
typedef struct {
    uint32_t max_texture_dimension_1d;
    uint32_t max_texture_dimension_2d;
    uint32_t max_texture_dimension_3d;
    uint32_t max_texture_array_layers;
    uint32_t max_bind_groups;
    uint32_t max_bind_groups_plus_vertex_buffers;
    uint32_t max_bindings_per_bind_group;
    uint32_t max_dynamic_uniform_buffers_per_pipeline_layout;
    uint32_t max_dynamic_storage_buffers_per_pipeline_layout;
    uint32_t max_sampled_textures_per_shader_stage;
    uint32_t max_samplers_per_shader_stage;
    uint32_t max_storage_buffers_per_shader_stage;
    uint32_t max_storage_textures_per_shader_stage;
    uint32_t max_uniform_buffers_per_shader_stage;
    uint64_t max_uniform_buffer_binding_size;
    uint64_t max_storage_buffer_binding_size;
    uint32_t min_uniform_buffer_offset_alignment;
    uint32_t min_storage_buffer_offset_alignment;
    uint32_t max_vertex_buffers;
    uint64_t max_buffer_size;
    uint32_t max_vertex_attributes;
    uint32_t max_vertex_buffer_array_stride;
    uint32_t max_inter_stage_shader_variables;
    uint32_t max_color_attachments;
    uint32_t max_color_attachment_bytes_per_sample;
    uint32_t max_compute_workgroup_storage_size;
    uint32_t max_compute_invocations_per_workgroup;
    uint32_t max_compute_workgroup_size_x;
    uint32_t max_compute_workgroup_size_y;
    uint32_t max_compute_workgroup_size_z;
    uint32_t max_compute_workgroups_per_dimension;
} gpu_supported_limits;

// Feature support
typedef struct {
    bool depth_clip_control;
    bool depth32float_stencil8;
    bool texture_compression_bc;
    bool texture_compression_bc_sliced3d;
    bool texture_compression_etc2;
    bool texture_compression_astc;
    bool timestamp_query;
    bool indirect_first_instance;
    bool shader_f16;
    bool rg11b10ufloat_renderable;
    bool bgra8unorm_storage;
    bool float32_filterable;
    bool clip_distances;
    bool dual_source_blending;
} gpu_supported_features;

// Adapter info
typedef struct {
    char *vendor;
    char *architecture;
    char *device;
    char *description;
} gpu_adapter_info;

// Request and configuration types
typedef struct {
    gpu_power_preference power_preference;
    bool force_fallback_adapter;
} gpu_request_adapter_options;

typedef struct {
    uint64_t size;
    uint32_t usage;
    bool mapped_at_creation;
} gpu_buffer_descriptor;

typedef struct {
    gpu_texture_format format;
    gpu_extent3_d size;
    uint32_t mip_level_count;
    uint32_t sample_count;
    uint32_t dimension;
    uint32_t usage;
    const char* label;
} gpu_texture_descriptor;

typedef struct {
    gpu_texture_format format;
    uint32_t usage;
    const gpu_texture_format* view_formats;
    uint32_t view_formats_length;
    uint32_t color_space;
    uint32_t alpha_mode;
} gpu_device_configuration;

// Function types for backend API
typedef wasi_webgpu_error (*BACKEND_INITIALIZE)(void **);
typedef wasi_webgpu_error (*BACKEND_DEINITIALIZE)(void *);
typedef wasi_webgpu_error (*GET_GPU)(void *ctx, void **gpu);
typedef wasi_webgpu_error (*REQUEST_ADAPTER)(void *ctx, void *gpu, gpu_request_adapter_options *options, void **adapter);
typedef wasi_webgpu_error (*REQUEST_DEVICE)(void *ctx, void *adapter, void **device);
typedef wasi_webgpu_error (*CREATE_BUFFER)(void *ctx, void *device, gpu_buffer_descriptor *desc, void **buffer);
typedef wasi_webgpu_error (*CREATE_TEXTURE)(void *ctx, void *device, gpu_texture_descriptor *desc, void **texture);
typedef wasi_webgpu_error (*CREATE_SHADER_MODULE)(void *ctx, void *device, gpu_shader_module_descriptor *desc, void **shader);
typedef wasi_webgpu_error (*CREATE_COMMAND_ENCODER)(void *ctx, void *device, const char *label, void **encoder);
typedef wasi_webgpu_error (*CREATE_RENDER_BUNDLE_ENCODER)(void *ctx, void *device, gpu_render_bundle_encoder_descriptor *desc, void **encoder);

typedef struct {
    BACKEND_INITIALIZE init;
    BACKEND_DEINITIALIZE deinit;
    GET_GPU get_gpu;
    REQUEST_ADAPTER request_adapter;
    REQUEST_DEVICE request_device;
    CREATE_BUFFER create_buffer;
    CREATE_TEXTURE create_texture;
    CREATE_SHADER_MODULE create_shader_module;
    CREATE_COMMAND_ENCODER create_command_encoder;
    CREATE_RENDER_BUNDLE_ENCODER create_render_bundle_encoder;
} api_function;

// Enums
enum gpu_compare_function {
    gpu_compare_function_never,
    gpu_compare_function_less,
    gpu_compare_function_equal,
    gpu_compare_function_less_equal,
    gpu_compare_function_greater,
    gpu_compare_function_not_equal,
    gpu_compare_function_greater_equal,
    gpu_compare_function_always,
};
typedef enum gpu_compare_function gpu_compare_function_t;

enum gpu_blend_operation {
    gpu_blend_operation_add,
    gpu_blend_operation_subtract,
    gpu_blend_operation_reverse_subtract,
    gpu_blend_operation_min,
    gpu_blend_operation_max,
};
typedef enum gpu_blend_operation gpu_blend_operation_t;

enum gpu_blend_factor {
    gpu_blend_factor_zero,
    gpu_blend_factor_one,
    gpu_blend_factor_src,
    gpu_blend_factor_one_minus_src,
    gpu_blend_factor_src_alpha,
    gpu_blend_factor_one_minus_src_alpha,
    gpu_blend_factor_dst,
    gpu_blend_factor_one_minus_dst,
    gpu_blend_factor_dst_alpha,
    gpu_blend_factor_one_minus_dst_alpha,
    gpu_blend_factor_src_alpha_saturated,
    gpu_blend_factor_constant,
    gpu_blend_factor_one_minus_constant,
    gpu_blend_factor_src1,
    gpu_blend_factor_one_minus_src1,
    gpu_blend_factor_src1_alpha,
    gpu_blend_factor_one_minus_src1_alpha,
};
typedef enum gpu_blend_factor gpu_blend_factor_t;

// Option type helpers
#define DEFINE_OPTION_TYPE(T) \
typedef struct { \
    bool has_value; \
    T value; \
} option_##T;

// Define basic option types
DEFINE_OPTION_TYPE(bool)
DEFINE_OPTION_TYPE(uint32_t)
DEFINE_OPTION_TYPE(uint64_t)
DEFINE_OPTION_TYPE(float)
DEFINE_OPTION_TYPE(double)
DEFINE_OPTION_TYPE(gpu_texture_format)

// Define option types for enums
typedef struct {
    bool has_value;
    enum gpu_compare_function value;
} option_gpu_compare_function;

typedef struct {
    bool has_value;
    enum gpu_blend_operation value;
} option_gpu_blend_operation;

typedef struct {
    bool has_value;
    enum gpu_blend_factor value;
} option_gpu_blend_factor;

// Structs that use enums
struct gpu_blend_component {
    enum gpu_blend_operation operation;
    enum gpu_blend_factor src_factor;
    enum gpu_blend_factor dst_factor;
};

struct gpu_blend_state {
    struct gpu_blend_component color;
    struct gpu_blend_component alpha;
};

struct gpu_color_target_state {
    gpu_texture_format format;
    struct {
        bool has_value;
        struct gpu_blend_state value;
    } blend;
    struct {
        bool has_value;
        uint32_t value;
    } write_mask;
};

typedef struct {
    bool has_value;
    struct gpu_color_target_state value;
} option_gpu_color_target_state;

// List type helpers
#define DEFINE_LIST_TYPE(T) \
typedef struct { \
    T* data; \
    uint32_t length; \
} list_##T;

DEFINE_LIST_TYPE(gpu_texture_format)
DEFINE_LIST_TYPE(gpu_bind_group_layout)
DEFINE_LIST_TYPE(gpu_compilation_message)
DEFINE_LIST_TYPE(option_gpu_color_target_state)

#endif /* WASI_WEBGPU_TYPES_H */
