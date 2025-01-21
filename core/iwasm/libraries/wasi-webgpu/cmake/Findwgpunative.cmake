# Copyright (C) 2019 Intel Corporation. All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

include(FetchContent)

set(WGPUNATIVE_SOURCE_DIR "${WAMR_ROOT_DIR}/core/deps/wgpu-native")
if(EXISTS ${WGPUNATIVE_SOURCE_DIR})
  message("Use existed source code under ${WGPUNATIVE_SOURCE_DIR}")
  FetchContent_Declare(
    wgpunative
    SOURCE_DIR     ${WGPUNATIVE_SOURCE_DIR}
  )
else()
  message("download source code and store it at ${WGPUNATIVE_SOURCE_DIR}")
  FetchContent_Declare(
    wgpunative
    GIT_REPOSITORY https://github.com/gfx-rs/wgpu-native.git
    GIT_TAG        v22.1.0.5
    SOURCE_DIR     ${WGPUNATIVE_SOURCE_DIR}
  )
endif()

FetchContent_MakeAvailable(wgpunative)
