use std::ffi::CStr;
use std::os::raw::c_char;
use std::sync::Arc;
use wgpu::{self, Instance, Device, Queue, Buffer};

#[repr(C)]
pub struct AdapterOptions {
    pub power_preference: u32,
    pub force_fallback_adapter: bool,
}

#[repr(C)]
pub struct BufferDescriptor {
    pub label: *const c_char,
    pub size: usize,
    pub usage: u32,
    pub mapped_at_creation: bool,
}

// FFI functions

#[no_mangle]
pub extern "C" fn rust_webgpu_device_create(
    options: *const AdapterOptions,
    device_ptr: *mut u64,
) -> i32 {
    if options.is_null() || device_ptr.is_null() {
        return 1; // Invalid argument
    }

    let options = unsafe { &*options };

    // Create instance and request adapter
    let instance = Instance::new(&wgpu::InstanceDescriptor::default());
    
    let power_preference = match options.power_preference {
        1 => wgpu::PowerPreference::LowPower,
        2 => wgpu::PowerPreference::HighPerformance,
        _ => wgpu::PowerPreference::default(),
    };

    let adapter = pollster::block_on(instance.request_adapter(
        &wgpu::RequestAdapterOptions {
            power_preference,
            force_fallback_adapter: options.force_fallback_adapter,
            compatible_surface: None,
        },
    ));

    let adapter = match adapter {
        Some(adapter) => adapter,
        None => return 2, // Failed to create adapter
    };

    // Request device
    let (device, queue) = pollster::block_on(adapter.request_device(
        &wgpu::DeviceDescriptor {
            label: None,
            required_features: wgpu::Features::empty(),
            required_limits: wgpu::Limits::default(),
            memory_hints: Default::default(),
        },
        None,
    )).unwrap();

    // Store device and queue in an Arc
    let device_queue = Arc::new((device, queue));
    unsafe {
        *device_ptr = Box::into_raw(Box::new(device_queue)) as u64;
    }

    0 // Success
}

#[no_mangle]
pub extern "C" fn rust_webgpu_device_destroy(device_ptr: u64) {
    if device_ptr != 0 {
        unsafe {
            drop(Box::from_raw(device_ptr as *mut Arc<(Device, Queue)>));
        }
    }
}

#[no_mangle]
pub extern "C" fn rust_webgpu_buffer_create(
    device_ptr: u64,
    desc: *const BufferDescriptor,
    buffer_ptr: *mut u64,
) -> i32 {
    if device_ptr == 0 || desc.is_null() || buffer_ptr.is_null() {
        return 1; // Invalid argument
    }

    let device_queue = unsafe { &*(device_ptr as *const Arc<(Device, Queue)>) };
    let desc = unsafe { &*desc };

    let label = if desc.label.is_null() {
        None
    } else {
        Some(unsafe { CStr::from_ptr(desc.label) }.to_str().unwrap())
    };

    let buffer = device_queue.0.create_buffer(&wgpu::BufferDescriptor {
        label,
        size: desc.size as u64,
        usage: wgpu::BufferUsages::from_bits_truncate(desc.usage),
        mapped_at_creation: desc.mapped_at_creation,
    });

    let buffer = Arc::new(buffer);
    unsafe {
        *buffer_ptr = Box::into_raw(Box::new(buffer)) as u64;
    }

    0 // Success
}

#[no_mangle]
pub extern "C" fn rust_webgpu_buffer_write(
    buffer_ptr: u64,
    data: *const u8,
    size: usize,
    offset: usize,
) -> i32 {
    if buffer_ptr == 0 || data.is_null() {
        return 1; // Invalid argument
    }

    let buffer = unsafe { &*(buffer_ptr as *const Arc<Buffer>) };
    let data_slice = unsafe { std::slice::from_raw_parts(data, size) };

    buffer.slice(offset as u64..offset as u64 + size as u64)
        .get_mapped_range_mut()
        .copy_from_slice(data_slice);

    0 // Success
}

#[no_mangle]
pub extern "C" fn rust_webgpu_buffer_destroy(buffer_ptr: u64) {
    if buffer_ptr != 0 {
        unsafe {
            drop(Box::from_raw(buffer_ptr as *mut Arc<Buffer>));
        }
    }
}

#[no_mangle]
pub extern "C" fn rust_webgpu_instance_request_adapter(adapter_ptr: *mut u64) -> i32 {
    if adapter_ptr.is_null() {
        return 1; // Invalid argument
    }

    let instance = Instance::new(&wgpu::InstanceDescriptor::default());
    
    let adapter = pollster::block_on(instance.request_adapter(
        &wgpu::RequestAdapterOptions::default(),
    ));

    match adapter {
        Some(adapter) => {
            unsafe {
                *adapter_ptr = Box::into_raw(Box::new(adapter)) as u64;
            }
            0 // Success
        }
        None => 2, // Failed to create adapter
    }
}

#[no_mangle]
pub extern "C" fn rust_webgpu_adapter_request_device(
    adapter_ptr: u64,
    device_ptr: *mut u64,
) -> i32 {
    if adapter_ptr == 0 || device_ptr.is_null() {
        return 1; // Invalid argument
    }

    let adapter = unsafe { &*(adapter_ptr as *const wgpu::Adapter) };
    
    let (device, queue) = match pollster::block_on(adapter.request_device(
        &wgpu::DeviceDescriptor {
            label: None,
            required_features: wgpu::Features::empty(),
            required_limits: wgpu::Limits::default(),
            memory_hints: Default::default(),
        },
        None,
    )) {
        Ok((device, queue)) => (device, queue),
        Err(_) => return 2, // Failed to create device
    };

    // Store device and queue in an Arc
    let device_queue = Arc::new((device, queue));
    unsafe {
        *device_ptr = Box::into_raw(Box::new(device_queue)) as u64;
    }

    0 // Success
}

#[no_mangle]
pub extern "C" fn rust_webgpu_adapter_enumerate_features(
    adapter_ptr: u64,
    features_ptr: *mut u32,
    count_ptr: *mut u32,
) -> i32 {
    if adapter_ptr == 0 || features_ptr.is_null() || count_ptr.is_null() {
        return 1; // Invalid argument
    }

    let adapter = unsafe { &*(adapter_ptr as *const wgpu::Adapter) };
    let features = adapter.features();
    
    unsafe {
        *count_ptr = features.bits().try_into().unwrap_or(0);
        *features_ptr = features.bits().try_into().unwrap_or(0);
    }

    0 // Success
}

// Add more FFI functions for shader, pipeline, and command operations
