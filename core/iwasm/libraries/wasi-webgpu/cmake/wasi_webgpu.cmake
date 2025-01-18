# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

# wasi-webgpu general
set(WASI_WEBGPU_ROOT ${CMAKE_CURRENT_LIST_DIR}/..)
set(WASI_WEBGPU_SOURCES
  ${WASI_WEBGPU_ROOT}/src/wasi_webgpu.c
)
include_directories(${WASI_WEBGPU_ROOT}/include)
add_compile_definitions(
  $<$<CONFIG:Debug>:WEBGPU_LOG_LEVEL=0>
  $<$<CONFIG:Release>:WEBGPU_LOG_LEVEL=2>
)

# Find Rust toolchain
find_program(CARGO_EXECUTABLE cargo)
if(NOT CARGO_EXECUTABLE)
    message(FATAL_ERROR "Cargo not found. Please install Rust toolchain.")
endif()

add_library(iwasm_wasi_webgpu STATIC)

target_include_directories(iwasm_wasi_webgpu
    PUBLIC
    ${WASI_WEBGPU_ROOT}/include
    PRIVATE
    ${WASI_WEBGPU_ROOT}/src
    ${WAMR_ROOT_DIR}/core/iwasm/include
    ${WAMR_ROOT_DIR}/core/shared/utils
    ${WAMR_ROOT_DIR}/core/shared/platform/include
)

file(GLOB_RECURSE SOURCE_ALL
    ${WASI_WEBGPU_ROOT}/src/*.c
)

target_sources(iwasm_wasi_webgpu
    PRIVATE
    ${SOURCE_ALL}
)

# Build Rust FFI library
set(RUST_TARGET_DIR ${CMAKE_CURRENT_BINARY_DIR}/rust)
file(MAKE_DIRECTORY ${RUST_TARGET_DIR})

add_custom_target(wasi_webgpu_rust
    COMMAND ${CARGO_EXECUTABLE} build --release --target-dir ${RUST_TARGET_DIR}
    WORKING_DIRECTORY ${WASI_WEBGPU_ROOT}/rust
)

# Link Rust library
add_dependencies(iwasm_wasi_webgpu wasi_webgpu_rust)

set(RUST_LIB_NAME "${RUST_TARGET_DIR}/release/libwasi_webgpu_rust.a")

target_link_libraries(iwasm_wasi_webgpu
    PRIVATE
    ${RUST_LIB_NAME}
)

# Set output name explicitly to match what iwasm expects
set_target_properties(iwasm_wasi_webgpu PROPERTIES
    OUTPUT_NAME "iwasm_wasi_webgpu"
)
