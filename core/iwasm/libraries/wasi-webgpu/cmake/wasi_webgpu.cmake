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
      ${WGPUNATIVE_INCLUDE_DIRS}
  )

  # Platform specific settings
  if(APPLE)
    set(IWASM_LIB_NAME "libiwasm.dylib")
  elseif(UNIX)
    set(IWASM_LIB_NAME "libiwasm.so")
  endif()

  link_directories(${CMAKE_BINARY_DIR})

  target_link_libraries(
    wasi_webgpu_wgpunative
    PUBLIC
      ${CMAKE_BINARY_DIR}/${IWASM_LIB_NAME}
      ${WGPUNATIVE_LIBRARIES}
  )

  # Add runtime path for shared libraries
  if(APPLE)
    set_target_properties(wasi_webgpu_wgpunative PROPERTIES
      INSTALL_RPATH "@loader_path"
    )
  elseif(UNIX)
    set_target_properties(wasi_webgpu_wgpunative PROPERTIES
      INSTALL_RPATH "$ORIGIN"
      LINK_FLAGS "-Wl,-rpath,$ORIGIN"
    )
  endif()

  add_dependencies(wasi_webgpu_wgpunative iwasm_shared)

  install(TARGETS wasi_webgpu_wgpunative DESTINATION lib)
endif()
