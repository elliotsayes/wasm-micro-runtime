use wgpu;

async fn run() {
    // Create instance
    let instance = wgpu::Instance::new(wgpu::InstanceDescriptor {
        backends: wgpu::Backends::all(),
        dx12_shader_compiler: Default::default(),
    });

    // Request adapter
    let adapter = instance
        .request_adapter(&wgpu::RequestAdapterOptions {
            power_preference: wgpu::PowerPreference::default(),
            force_fallback_adapter: false,
            compatible_surface: None,
        })
        .await
        .unwrap();

    // Create device and queue
    let (device, queue) = adapter
        .request_device(
            &wgpu::DeviceDescriptor {
                label: None,
                features: wgpu::Features::empty(),
                limits: wgpu::Limits::default(),
            },
            None,
        )
        .await
        .unwrap();

    // Create vertex buffer
    let vertex_buffer = device.create_buffer(&wgpu::BufferDescriptor {
        label: Some("Vertex Buffer"),
        size: 9 * std::mem::size_of::<f32>() as u64,
        usage: wgpu::BufferUsages::VERTEX,
        mapped_at_creation: true,
    });

    // Triangle vertices
    let vertices = [
        -0.5f32, -0.5, 0.0,  // Bottom left
         0.5,    -0.5, 0.0,  // Bottom right
         0.0,     0.5, 0.0,  // Top
    ];

    // Write vertices to buffer
    {
        let mut vertex_map = vertex_buffer.slice(..).get_mapped_range_mut();
        vertex_map.copy_from_slice(bytemuck::cast_slice(&vertices));
    }
    vertex_buffer.unmap();

    println!("Triangle example completed successfully!");
}

fn main() {
    pollster::block_on(run());
}
