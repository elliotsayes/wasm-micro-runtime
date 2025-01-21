# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

#
# wasi-webgpu general
set(WASI_WEBGPU_ROOT ${CMAKE_CURRENT_LIST_DIR}/..)
set(WASI_WEBGPU_SOURCES
  ${WASI_WEBGPU_ROOT}/src/wasi_webgpu.c
  ${WASI_WEBGPU_ROOT}/src/utils/wasi_webgpu_app_native.c
)
include_directories(${WASI_WEBGPU_ROOT}/include)
add_compile_definitions(
  $<$<CONFIG:Debug>:NN_LOG_LEVEL=0>
  $<$<CONFIG:Release>:NN_LOG_LEVEL=2>
)

# - wgpunative

if(WAMR_BUILD_WASI_WEBGPU_WGPU EQUAL 1)
  find_package(wgpunative REQUIRED)

  add_library(
    wasi_webgpu_wgpunative
    SHARED
      ${WASI_WEBGPU_ROOT}/src/wasi_webgpu_wgpunative.c
  )

  target_include_directories(
    wasi_webgpu_wgpunative
    PUBLIC
        "${WGPUNATIVE_SOURCE_DIR}/ffi"
        "${WGPUNATIVE_SOURCE_DIR}/ffi/webgpu-headers"
  )

  target_link_libraries(
    wasi_webgpu_wgpunative
    PUBLIC
      libiwasm
      wgpu-native
  )

  install(TARGETS wasi_webgpu_wgpunative DESTINATION lib)
endif()
