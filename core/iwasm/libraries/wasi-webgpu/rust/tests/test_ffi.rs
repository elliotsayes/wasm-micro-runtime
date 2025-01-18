extern crate wasi_webgpu_rust;

#[cfg(test)]
mod tests {
    use std::ptr;
    use wasi_webgpu_rust::{
        AdapterOptions,
        BufferDescriptor,
        rust_webgpu_device_create,
        rust_webgpu_device_destroy,
        rust_webgpu_buffer_create,
        rust_webgpu_buffer_write,
        rust_webgpu_buffer_destroy,
    };

    #[test]
    fn test_device_creation() {
        let options = AdapterOptions {
            power_preference: 2, // High performance
            force_fallback_adapter: false,
        };
        
        let mut device_ptr: u64 = 0;
        let result = rust_webgpu_device_create(&options, &mut device_ptr);
        
        assert_eq!(result, 0);
        assert_ne!(device_ptr, 0);
        
        rust_webgpu_device_destroy(device_ptr);
    }

    #[test]
    fn test_buffer_operations() {
        // First create a device
        let options = AdapterOptions {
            power_preference: 2,
            force_fallback_adapter: false,
        };
        
        let mut device_ptr: u64 = 0;
        let result = rust_webgpu_device_create(&options, &mut device_ptr);
        assert_eq!(result, 0);
        
        // Create a buffer
        let buffer_desc = BufferDescriptor {
            label: ptr::null(),
            size: 1024,
            usage: 0x0008, // COPY_DST (8)
            mapped_at_creation: true,
        };
        
        let mut buffer_ptr: u64 = 0;
        let result = rust_webgpu_buffer_create(device_ptr, &buffer_desc, &mut buffer_ptr);
        assert_eq!(result, 0);
        assert_ne!(buffer_ptr, 0);
        
        // Write to buffer
        let data: Vec<u8> = vec![1, 2, 3, 4];
        let result = rust_webgpu_buffer_write(buffer_ptr, data.as_ptr(), data.len(), 0);
        assert_eq!(result, 0);
        
        // Clean up
        rust_webgpu_buffer_destroy(buffer_ptr);
        rust_webgpu_device_destroy(device_ptr);
    }

    #[test]
    fn test_invalid_device() {
        let buffer_desc = BufferDescriptor {
            label: ptr::null(),
            size: 1024,
            usage: 0x21,
            mapped_at_creation: false,
        };
        
        let mut buffer_ptr: u64 = 0;
        let result = rust_webgpu_buffer_create(0, &buffer_desc, &mut buffer_ptr);
        assert_eq!(result, 1); // Should fail with invalid argument
    }

    #[test]
    fn test_null_arguments() {
        let mut device_ptr: u64 = 0;
        let result = rust_webgpu_device_create(ptr::null(), &mut device_ptr);
        assert_eq!(result, 1); // Should fail with invalid argument
    }
}
