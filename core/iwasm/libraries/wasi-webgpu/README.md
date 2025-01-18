# WASI-WebGPU

WASI-WebGPU is a WebAssembly System Interface (WASI) implementation for WebGPU, powered by Rust's wgpu library. It provides a safe and efficient way to access GPU capabilities from WebAssembly modules.

## Features

- WebGPU device creation and management
- Buffer operations (create, write, destroy)
- Shader module support
- Render pipeline creation
- Command buffer encoding and submission
- Thread-safe resource management
- Memory-safe operations through Rust

## Prerequisites

- Rust toolchain (for building the FFI layer)
- CMake 3.13 or higher
- A GPU supporting Vulkan, Metal, D3D12, or WebGPU

## Building

Enable WASI-WebGPU in WAMR by specifying it in the CMake configuration:

```cmake
set(WAMR_BUILD_WASI_WEBGPU 1)
```

Or via command line:

```bash
cmake -DWAMR_BUILD_WASI_WEBGPU=1 ...
```

## Usage

Include the WASI-WebGPU header in your WebAssembly application:

```c
#include <wasi_webgpu.h>
```

Basic example:

```c
// Create device
adapter_options options = {
    .power_preference = WASI_WEBGPU_POWER_PREFERENCE_HIGH_PERFORMANCE,
    .force_fallback_adapter = false
};
device_handle device;
wasi_webgpu_error err = device_create(&options, &device);

// Create buffer
buffer_descriptor desc = {
    .label = "My Buffer",
    .size = 1024,
    .usage = WASI_WEBGPU_BUFFER_USAGE_VERTEX | WASI_WEBGPU_BUFFER_USAGE_COPY_DST,
    .mapped_at_creation = false
};
buffer_handle buffer;
err = buffer_create(device, &desc, &buffer);

// Write data to buffer
uint8_t data[1024] = { /* ... */ };
err = buffer_write(buffer, data, sizeof(data), 0);
```

## Architecture

The library consists of three main components:

1. **C API Layer** (`wasi_webgpu.h`, `wasi_webgpu_types.h`)
   - Defines the public WebGPU interface
   - Handles resource management and type definitions

2. **Core Implementation** (`wasi_webgpu.c`)
   - Manages WebGPU contexts
   - Tracks resources and handles
   - Provides thread safety

3. **Rust FFI Layer** (`rust/src/lib.rs`)
   - Implements the actual WebGPU functionality using wgpu
   - Ensures memory safety through Rust's ownership system
   - Handles conversion between C and Rust types

## Safety

The library ensures safety through several mechanisms:

- Resource tracking and cleanup
- Thread-safe operations
- Memory safety through Rust
- Validation of all operations
- Error handling and reporting

## Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

## License

This project is licensed under the Apache License 2.0 with LLVM Exception.
