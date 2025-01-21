/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#ifndef WASI_WEBGPU_HOST_H
#define WASI_WEBGPU_HOST_H

#include "lib_export.h"

uint32_t
get_wasi_webgpu_export_apis(NativeSymbol **p_native_symbols);

bool
wasi_webgpu_initialize();

void
wasi_webgpu_destroy();

#endif /* WASI_WEBGPU_HOST_H */