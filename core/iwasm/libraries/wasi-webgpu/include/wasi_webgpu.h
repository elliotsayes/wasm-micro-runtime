/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

/**
 * Following definition from:
 * [Oct 25th, 2022]
 * https://github.com/WebAssembly/wasi-webgpu/blob/main/webgpu/webgpu.wit
 */

#ifndef WASI_WEBGPU_H
#define WASI_WEBGPU_H

#include <stdint.h>
#include "wasi_webgpu_types.h"

/**
 * GPU Instance Management
 */

/**
 * @brief Get the GPU instance.
 *
 * @param gpu Pointer to receive the GPU instance.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_get_gpu(void **gpu)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Get the preferred canvas format.
 *
 * @param gpu GPU instance.
 * @param format Pointer to receive the preferred format.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_get_preferred_canvas_format(void *gpu, gpu_texture_format *format)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Get supported WGSL language features.
 *
 * @param gpu GPU instance.
 * @param features Pointer to receive the features.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_get_wgsl_language_features(void *gpu, void **features)
    __attribute__((import_module("wasi_webgpu")));

/**
 * Adapter Management
 */

/**
 * @brief Request a GPU adapter.
 *
 * @param gpu GPU instance.
 * @param options Adapter options.
 * @param adapter Pointer to receive the adapter.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_request_adapter(void *gpu, gpu_request_adapter_options *options, void **adapter)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Get adapter info.
 *
 * @param adapter GPU adapter.
 * @param info Pointer to receive adapter info.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_adapter_get_info(void *adapter, gpu_adapter_info *info)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Get adapter features.
 *
 * @param adapter GPU adapter.
 * @param features Pointer to receive supported features.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_adapter_get_features(void *adapter, gpu_supported_features *features)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Get adapter limits.
 *
 * @param adapter GPU adapter.
 * @param limits Pointer to receive supported limits.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_adapter_get_limits(void *adapter, gpu_supported_limits *limits)
    __attribute__((import_module("wasi_webgpu")));

/**
 * Device Management
 */

/**
 * @brief Request a GPU device from an adapter.
 *
 * @param adapter GPU adapter.
 * @param device Pointer to receive the device.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_request_device(void *adapter, void **device)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Configure the device for a graphics context.
 *
 * @param device GPU device.
 * @param config Device configuration.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_device_configure(void *device, gpu_device_configuration *config)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Connect a graphics context to a device.
 *
 * @param device GPU device.
 * @param context Graphics context.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_device_connect_graphics_context(void *device, void *context)
    __attribute__((import_module("wasi_webgpu")));

/**
 * Resource Creation
 */

/**
 * @brief Create a GPU buffer.
 *
 * @param device GPU device.
 * @param desc Buffer descriptor.
 * @param buffer Pointer to receive the buffer.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_device_create_buffer(void *device, gpu_buffer_descriptor *desc, void **buffer)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Create a GPU texture.
 *
 * @param device GPU device.
 * @param desc Texture descriptor.
 * @param texture Pointer to receive the texture.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_device_create_texture(void *device, gpu_texture_descriptor *desc, void **texture)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Create a sampler.
 *
 * @param device GPU device.
 * @param desc Sampler descriptor.
 * @param sampler Pointer to receive the sampler.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_device_create_sampler(void *device, void *desc, void **sampler)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Create a bind group layout.
 *
 * @param device GPU device.
 * @param desc Bind group layout descriptor.
 * @param layout Pointer to receive the bind group layout.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_device_create_bind_group_layout(void *device, gpu_bind_group_layout_descriptor *desc, void **layout)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Create a pipeline layout.
 *
 * @param device GPU device.
 * @param desc Pipeline layout descriptor.
 * @param layout Pointer to receive the pipeline layout.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_device_create_pipeline_layout(void *device, gpu_pipeline_layout_descriptor *desc, void **layout)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Create a bind group.
 *
 * @param device GPU device.
 * @param desc Bind group descriptor.
 * @param group Pointer to receive the bind group.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_device_create_bind_group(void *device, gpu_bind_group_descriptor *desc, void **group)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Create a shader module.
 *
 * @param device GPU device.
 * @param desc Shader module descriptor.
 * @param shader Pointer to receive the shader module.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_device_create_shader_module(void *device, gpu_shader_module_descriptor *desc, void **shader)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Create a compute pipeline.
 *
 * @param device GPU device.
 * @param desc Compute pipeline descriptor.
 * @param pipeline Pointer to receive the compute pipeline.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_device_create_compute_pipeline(void *device, gpu_compute_pipeline_descriptor *desc, void **pipeline)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Create a compute pipeline asynchronously.
 *
 * @param device GPU device.
 * @param desc Compute pipeline descriptor.
 * @param pipeline Pointer to receive the compute pipeline.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_device_create_compute_pipeline_async(void *device, gpu_compute_pipeline_descriptor *desc, void **pipeline)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Create a render pipeline.
 *
 * @param device GPU device.
 * @param desc Render pipeline descriptor.
 * @param pipeline Pointer to receive the render pipeline.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_device_create_render_pipeline(void *device, gpu_render_pipeline_descriptor *desc, void **pipeline)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Create a render pipeline asynchronously.
 *
 * @param device GPU device.
 * @param desc Render pipeline descriptor.
 * @param pipeline Pointer to receive the render pipeline.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_device_create_render_pipeline_async(void *device, gpu_render_pipeline_descriptor *desc, void **pipeline)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Create a query set.
 *
 * @param device GPU device.
 * @param desc Query set descriptor.
 * @param query_set Pointer to receive the query set.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_device_create_query_set(void *device, gpu_query_set_descriptor *desc, void **query_set)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Create a command encoder.
 *
 * @param device GPU device.
 * @param label Optional label for the encoder.
 * @param encoder Pointer to receive the command encoder.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_device_create_command_encoder(void *device, const char *label, void **encoder)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Create a render bundle encoder.
 *
 * @param device GPU device.
 * @param desc Render bundle encoder descriptor.
 * @param encoder Pointer to receive the render bundle encoder.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_device_create_render_bundle_encoder(void *device, gpu_render_bundle_encoder_descriptor *desc, void **encoder)
    __attribute__((import_module("wasi_webgpu")));

/**
 * Texture Operations
 */

/**
 * @brief Create a texture from a graphics buffer.
 *
 * @param device GPU device.
 * @param buffer Graphics buffer.
 * @param texture Pointer to receive the texture.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_texture_from_graphics_buffer(void *device, void *buffer, void **texture)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Create a texture view.
 *
 * @param texture GPU texture.
 * @param desc Texture view descriptor.
 * @param view Pointer to receive the texture view.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_texture_create_view(void *texture, gpu_texture_view_descriptor *desc, void **view)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Destroy a texture.
 *
 * @param texture GPU texture.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_texture_destroy(void *texture)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Get texture width.
 *
 * @param texture GPU texture.
 * @param width Pointer to receive the width.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_texture_get_width(void *texture, uint32_t *width)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Get texture height.
 *
 * @param texture GPU texture.
 * @param height Pointer to receive the height.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_texture_get_height(void *texture, uint32_t *height)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Get texture depth or array layers.
 *
 * @param texture GPU texture.
 * @param depth_or_array_layers Pointer to receive the depth or array layers.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_texture_get_depth_or_array_layers(void *texture, uint32_t *depth_or_array_layers)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Get texture mip level count.
 *
 * @param texture GPU texture.
 * @param mip_level_count Pointer to receive the mip level count.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_texture_get_mip_level_count(void *texture, uint32_t *mip_level_count)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Get texture sample count.
 *
 * @param texture GPU texture.
 * @param sample_count Pointer to receive the sample count.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_texture_get_sample_count(void *texture, uint32_t *sample_count)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Get texture dimension.
 *
 * @param texture GPU texture.
 * @param dimension Pointer to receive the dimension.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_texture_get_dimension(void *texture, gpu_texture_dimension *dimension)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Get texture format.
 *
 * @param texture GPU texture.
 * @param format Pointer to receive the format.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_texture_get_format(void *texture, gpu_texture_format *format)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Get texture usage.
 *
 * @param texture GPU texture.
 * @param usage Pointer to receive the usage flags.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_texture_get_usage(void *texture, uint32_t *usage)
    __attribute__((import_module("wasi_webgpu")));

/**
 * Command Encoder Operations
 */

/**
 * @brief Begin a render pass.
 *
 * @param encoder Command encoder.
 * @param desc Render pass descriptor.
 * @param pass Pointer to receive the render pass encoder.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_command_encoder_begin_render_pass(void *encoder, gpu_render_pass_descriptor *desc, void **pass)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Begin a compute pass.
 *
 * @param encoder Command encoder.
 * @param desc Compute pass descriptor.
 * @param pass Pointer to receive the compute pass encoder.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_command_encoder_begin_compute_pass(void *encoder, gpu_compute_pass_descriptor *desc, void **pass)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Copy buffer to buffer.
 *
 * @param encoder Command encoder.
 * @param source Source buffer.
 * @param source_offset Source offset.
 * @param destination Destination buffer.
 * @param destination_offset Destination offset.
 * @param size Size to copy.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_command_encoder_copy_buffer_to_buffer(void *encoder, void *source, uint64_t source_offset,
                                                 void *destination, uint64_t destination_offset, uint64_t size)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Copy buffer to texture.
 *
 * @param encoder Command encoder.
 * @param source Source buffer copy view.
 * @param destination Destination texture copy view.
 * @param copy_size Copy size.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_command_encoder_copy_buffer_to_texture(void *encoder, gpu_image_copy_buffer *source,
                                                  gpu_image_copy_texture *destination, gpu_extent3_d *copy_size)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Copy texture to buffer.
 *
 * @param encoder Command encoder.
 * @param source Source texture copy view.
 * @param destination Destination buffer copy view.
 * @param copy_size Copy size.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_command_encoder_copy_texture_to_buffer(void *encoder, gpu_image_copy_texture *source,
                                                  gpu_image_copy_buffer *destination, gpu_extent3_d *copy_size)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Copy texture to texture.
 *
 * @param encoder Command encoder.
 * @param source Source texture copy view.
 * @param destination Destination texture copy view.
 * @param copy_size Copy size.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_command_encoder_copy_texture_to_texture(void *encoder, gpu_image_copy_texture *source,
                                                   gpu_image_copy_texture *destination, gpu_extent3_d *copy_size)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Push debug group.
 *
 * @param encoder Command encoder.
 * @param group_label Debug group label.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_command_encoder_push_debug_group(void *encoder, const char *group_label)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Pop debug group.
 *
 * @param encoder Command encoder.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_command_encoder_pop_debug_group(void *encoder)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Insert debug marker.
 *
 * @param encoder Command encoder.
 * @param marker_label Debug marker label.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_command_encoder_insert_debug_marker(void *encoder, const char *marker_label)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Finish command encoder.
 *
 * @param encoder Command encoder.
 * @param desc Command buffer descriptor.
 * @param buffer Pointer to receive the command buffer.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_command_encoder_finish(void *encoder, gpu_command_buffer_descriptor *desc, void **buffer)
    __attribute__((import_module("wasi_webgpu")));

/**
 * Error Handling
 */

/**
 * @brief Push an error scope.
 *
 * @param device GPU device.
 * @param filter Error filter.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_device_push_error_scope(void *device, gpu_error_filter filter)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Pop an error scope.
 *
 * @param device GPU device.
 * @param error Pointer to receive the error if any.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_device_pop_error_scope(void *device, void **error)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Get device lost info.
 *
 * @param device GPU device.
 * @param lost_info Pointer to receive the lost info.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_device_get_lost_info(void *device, void **lost_info)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Get uncaptured error callbacks.
 *
 * @param device GPU device.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_device_uncaptured_errors(void *device)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Get the device label.
 *
 * @param device GPU device.
 * @param label Pointer to receive the label string.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_device_get_label(void *device, char **label)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Set the device label.
 *
 * @param device GPU device.
 * @param label Label string.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_device_set_label(void *device, const char *label)
    __attribute__((import_module("wasi_webgpu")));

/**
 * Buffer Operations
 */

/**
 * @brief Map a GPU buffer.
 *
 * @param buffer GPU buffer.
 * @param mode Map mode flags.
 * @param offset Optional offset in bytes.
 * @param size Optional size in bytes.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_buffer_map_async(void *buffer, gpu_map_mode_flags mode, uint64_t offset, uint64_t size)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Get mapped range of a GPU buffer.
 *
 * @param buffer GPU buffer.
 * @param offset Optional offset in bytes.
 * @param size Optional size in bytes.
 * @param data Pointer to receive the mapped data.
 * @param data_size Size of the mapped data.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_buffer_get_mapped_range(void *buffer, uint64_t offset, uint64_t size,
                                   void **data, uint64_t *data_size)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Unmap a GPU buffer.
 *
 * @param buffer GPU buffer.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_buffer_unmap(void *buffer)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Get buffer size.
 *
 * @param buffer GPU buffer.
 * @param size Pointer to receive the size.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_buffer_get_size(void *buffer, uint64_t *size)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Get buffer usage.
 *
 * @param buffer GPU buffer.
 * @param usage Pointer to receive the usage flags.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_buffer_get_usage(void *buffer, uint32_t *usage)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Get buffer map state.
 *
 * @param buffer GPU buffer.
 * @param state Pointer to receive the map state.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_buffer_get_map_state(void *buffer, gpu_buffer_map_state *state)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Destroy a buffer.
 *
 * @param buffer GPU buffer.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_buffer_destroy(void *buffer)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Get buffer label.
 *
 * @param buffer GPU buffer.
 * @param label Pointer to receive the label string.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_buffer_get_label(void *buffer, char **label)
    __attribute__((import_module("wasi_webgpu")));

/**
 * @brief Set buffer label.
 *
 * @param buffer GPU buffer.
 * @param label Label string.
 * @return wasi_webgpu_error Execution status.
 */
wasi_webgpu_error
wasi_webgpu_buffer_set_label(void *buffer, const char *label)
    __attribute__((import_module("wasi_webgpu")));

/**
 * GPU Device functions
 */

/**
 * @brief Get the GPU queue.
 *
 * @param device GPU device.
 * @param queue Pointer to receive the queue.
 * @return wasi_webgpu_error Execution status.
 */
gpu_queue *
gpu_device_get_queue(gpu_device *device);

/**
 * GPU Shader Module functions
 */

/**
 * @brief Get the compilation info of a shader module.
 *
 * @param shader_module Shader module.
 * @param compilation_info Pointer to receive the compilation info.
 * @return wasi_webgpu_error Execution status.
 */
gpu_compilation_info *
gpu_shader_module_get_compilation_info(gpu_shader_module *shader_module);

/**
 * GPU Pipeline functions
 */

/**
 * @brief Get the bind group layout of a compute pipeline.
 *
 * @param pipeline Compute pipeline.
 * @param index Index of the bind group layout.
 * @param bind_group_layout Pointer to receive the bind group layout.
 * @return wasi_webgpu_error Execution status.
 */
gpu_bind_group_layout *
gpu_compute_pipeline_get_bind_group_layout(gpu_compute_pipeline *pipeline, uint32_t index);

/**
 * @brief Get the bind group layout of a render pipeline.
 *
 * @param pipeline Render pipeline.
 * @param index Index of the bind group layout.
 * @param bind_group_layout Pointer to receive the bind group layout.
 * @return wasi_webgpu_error Execution status.
 */
gpu_bind_group_layout *
gpu_render_pipeline_get_bind_group_layout(gpu_render_pipeline *pipeline, uint32_t index);

/**
 * GPU Queue functions
 */

/**
 * @brief Submit command buffers to a queue.
 *
 * @param queue GPU queue.
 * @param command_buffers List of command buffers to submit.
 * @return wasi_webgpu_error Execution status.
 */
void
gpu_queue_submit(gpu_queue *queue, list_gpu_command_buffer *command_buffers);

/**
 * @brief Write data to a buffer in a queue.
 *
 * @param queue GPU queue.
 * @param buffer Buffer to write to.
 * @param buffer_offset Offset in the buffer to write to.
 * @param data Data to write.
 * @param size Size of the data to write.
 * @return wasi_webgpu_error Execution status.
 */
void
gpu_queue_write_buffer(gpu_queue *queue, gpu_buffer *buffer, uint64_t buffer_offset, const uint8_t *data, size_t size);

/**
 * @brief Write data to a texture in a queue.
 *
 * @param queue GPU queue.
 * @param destination Destination texture copy view.
 * @param data Data to write.
 * @param data_size Size of the data to write.
 * @param data_layout Layout of the data to write.
 * @param write_size Size of the write operation.
 * @return wasi_webgpu_error Execution status.
 */
void
gpu_queue_write_texture(gpu_queue *queue, const gpu_image_copy_texture *destination, const void *data, size_t data_size, const gpu_texture_data_layout *data_layout, const gpu_extent3d *write_size);

/**
 * GPU Compilation Info functions
 */

/**
 * @brief Get the compilation messages of a compilation info.
 *
 * @param info Compilation info.
 * @param messages Pointer to receive the compilation messages.
 * @return wasi_webgpu_error Execution status.
 */
list_gpu_compilation_message *
gpu_compilation_info_get_messages(gpu_compilation_info *info);

/**
 * GPU Compilation Message functions
 */

/**
 * @brief Get the message of a compilation message.
 *
 * @param message Compilation message.
 * @param message_str Pointer to receive the message string.
 * @return wasi_webgpu_error Execution status.
 */
const char *
gpu_compilation_message_get_message(gpu_compilation_message *message);

/**
 * @brief Get the type of a compilation message.
 *
 * @param message Compilation message.
 * @param type Pointer to receive the message type.
 * @return wasi_webgpu_error Execution status.
 */
gpu_compilation_message_type
gpu_compilation_message_get_type(gpu_compilation_message *message);

/**
 * @brief Get the line number of a compilation message.
 *
 * @param message Compilation message.
 * @param line_num Pointer to receive the line number.
 * @return wasi_webgpu_error Execution status.
 */
uint64_t
gpu_compilation_message_get_line_num(gpu_compilation_message *message);

/**
 * @brief Get the line position of a compilation message.
 *
 * @param message Compilation message.
 * @param line_pos Pointer to receive the line position.
 * @return wasi_webgpu_error Execution status.
 */
uint64_t
gpu_compilation_message_get_line_pos(gpu_compilation_message *message);

/**
 * @brief Get the offset of a compilation message.
 *
 * @param message Compilation message.
 * @param offset Pointer to receive the offset.
 * @return wasi_webgpu_error Execution status.
 */
uint64_t
gpu_compilation_message_get_offset(gpu_compilation_message *message);

/**
 * @brief Get the length of a compilation message.
 *
 * @param message Compilation message.
 * @param length Pointer to receive the length.
 * @return wasi_webgpu_error Execution status.
 */
uint64_t
gpu_compilation_message_get_length(gpu_compilation_message *message);

#endif
