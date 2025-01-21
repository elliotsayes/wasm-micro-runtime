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

# Build wgpu-native using cargo
find_program(CARGO_EXECUTABLE cargo REQUIRED)
message(STATUS "Found cargo: ${CARGO_EXECUTABLE}")

# Add custom target to build wgpu-native
add_custom_target(build_wgpu_native
  COMMAND ${CARGO_EXECUTABLE} build --release
  WORKING_DIRECTORY ${WGPUNATIVE_SOURCE_DIR}
  COMMENT "Building wgpu-native with cargo"
)

# Set platform-specific library paths and names
if(APPLE)
  set(WGPU_NATIVE_LIB "${WGPUNATIVE_SOURCE_DIR}/target/release/libwgpu_native.dylib")
  set(WGPU_NATIVE_LINK_NAME "wgpu_native")
elseif(UNIX AND NOT APPLE)
  set(WGPU_NATIVE_LIB "${WGPUNATIVE_SOURCE_DIR}/target/release/libwgpu_native.so")
  set(WGPU_NATIVE_LINK_NAME "wgpu_native")
elseif(WIN32)
  set(WGPU_NATIVE_LIB "${WGPUNATIVE_SOURCE_DIR}/target/release/wgpu_native.dll")
  set(WGPU_NATIVE_LINK_NAME "wgpu_native")
endif()

# Create imported target for wgpu-native
add_library(wgpu-native SHARED IMPORTED GLOBAL)
set_target_properties(wgpu-native PROPERTIES
  IMPORTED_LOCATION "${WGPU_NATIVE_LIB}"
  IMPORTED_NO_SONAME TRUE
)

if(UNIX AND NOT APPLE)
  # On Linux, also set the SONAME
  set_target_properties(wgpu-native PROPERTIES
    IMPORTED_SONAME "libwgpu_native.so"
  )
endif()

add_dependencies(wgpu-native build_wgpu_native)

# Set include directories
set(WGPUNATIVE_INCLUDE_DIRS
  "${WGPUNATIVE_SOURCE_DIR}/ffi"
  "${WGPUNATIVE_SOURCE_DIR}/ffi/webgpu-headers"
)

# Export variables for use in parent scope
set(WGPUNATIVE_LIBRARIES wgpu-native)
set(WGPUNATIVE_FOUND TRUE)
