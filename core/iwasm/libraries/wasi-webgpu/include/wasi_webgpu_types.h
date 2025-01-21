/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#ifndef WASI_WEBGPU_TYPES_H
#define WASI_WEBGPU_TYPES_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    // TODO

    // No error occurred.
    success = 0,
} wasi_webgpu_error;

typedef enum {
    wgpunative,
    autodetect,
} backend_type;

typedef struct {
    uint8_t *buf;
    uint32_t size;
} mybuffer;

typedef struct {
    mybuffer *buf;
    uint32_t size;
} mybuffer_array;

// TODO
/* wasi-webgpu general APIs */
typedef wasi_webgpu_error (*BACKEND_INITIALIZE)(void **);
typedef wasi_webgpu_error (*BACKEND_DEINITIALIZE)(void *);

typedef struct {
    // TODO
    
    BACKEND_INITIALIZE init;
    BACKEND_DEINITIALIZE deinit;
} api_function;

#ifdef __cplusplus
}
#endif
#endif
