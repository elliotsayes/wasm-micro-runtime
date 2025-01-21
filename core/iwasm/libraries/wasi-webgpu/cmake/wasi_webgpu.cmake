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

#
# wasi-webgpu backends
#
# - tflite
if(WAMR_BUILD_WASI_WEBGPU_TFLITE EQUAL 1)
  find_package(tensorflow_lite REQUIRED)

  add_library(
    wasi_webgpu_tflite
    SHARED
      ${WASI_WEBGPU_ROOT}/src/wasi_webgpu_tensorflowlite.cpp
  )

  target_include_directories(
    wasi_webgpu_tflite
    PUBLIC
      ${tensorflow_lite_SOURCE_DIR}
  )

  target_link_libraries(
    wasi_webgpu_tflite
    PUBLIC
      libiwasm
      tensorflow-lite
  )

  install(TARGETS wasi_webgpu_tflite DESTINATION lib)
endif()

# - openvino
if(WAMR_BUILD_WASI_WEBGPU_OPENVINO EQUAL 1)
  if(NOT DEFINED ENV{OpenVINO_DIR})
    message(FATAL_ERROR
        "OpenVINO_DIR is not defined. "
        "Please follow https://docs.openvino.ai/2024/get-started/install-openvino.html,"
        "install openvino, and set environment variable OpenVINO_DIR."
        "Like OpenVINO_DIR=/usr/lib/openvino-2023.2/ cmake ..."
        "Or OpenVINO_DIR=/opt/intel/openvino/ cmake ..."
    )
  endif()

  list(APPEND CMAKE_MODULE_PATH $ENV{OpenVINO_DIR})
  # Find OpenVINO
  find_package(OpenVINO REQUIRED COMPONENTS Runtime)

  add_library(
    wasi_webgpu_openvino
    SHARED
      ${WASI_WEBGPU_ROOT}/src/wasi_webgpu_openvino.c
  )

  target_link_libraries(
    wasi_webgpu_openvino
    PUBLIC
      libiwasm
      openvino::runtime
      openvino::runtime::c
  )

  install(TARGETS wasi_webgpu_openvino DESTINATION lib)
endif()

# - llamacpp

if(WAMR_BUILD_WASI_WEBGPU_LLAMACPP EQUAL 1)
  find_package(cjson REQUIRED)
  find_package(llamacpp REQUIRED)

  add_library(
    wasi_webgpu_llamacpp
    SHARED
      ${WASI_WEBGPU_ROOT}/src/wasi_webgpu_llamacpp.c
  )

  target_include_directories(
    wasi_webgpu_llamacpp
    PUBLIC
      ${cjson_SOURCE_DIR}
  )

  target_link_libraries(
    wasi_webgpu_llamacpp
    PUBLIC
      libiwasm
      cjson
      common
      ggml
      llama
  )

  install(TARGETS wasi_webgpu_llamacpp DESTINATION lib)
endif()
