#include "cube.hpp"
#include <arena.hpp>
#include <string.hpp>
#include <webgpu.hpp>

#include <math/matrix.hpp>

struct TextureSize {
  int x, y, z;
};

extern "C" void request_animation_frame(void *);

extern "C" {
Arena transfer_arena;
}

const char *vertexCode =
    R"(
    struct VertexOut {
        @builtin(position) Position: vec4f,
        @location(0) color: vec4f,
      }; 
    struct VertexIn {
      @location(0) position: vec4f,
      @location(1) color: vec4f,
      @location(2) uv: vec2f
    };
    @vertex fn main( vtx: VertexIn ) -> VertexOut {
      var vsOut: VertexOut;
      vsOut.Position = vec4f((vtx.position.x)/vtx.position.z, (vtx.position.y)/vtx.position.z, 0.0, 1.0);
      vsOut.color = vtx.color;
      return vsOut;
    })";

const char *fragmentCode =
    R"(
     struct FragmentIn {
        @location(0) color: vec4f,
      }; 
      @fragment fn main(fsInput: FragmentIn) -> @location(0) vec4f {
        return fsInput.color;//fsInput.color;
      })";

using namespace wgpu;

Device device{};
Pipeline pipeline{};
Queue queue{};
Buffer vtxBuffer{};
Buffer uniformBuffer{};
BindGroup bindGroup;

void build_pipeline() {

  ColorTargetState target;
  target.format = GetPreferredCanvasFormat();

  VertexBufferLayout bufferLayout;
  bufferLayout.arrayStride = cubeVertexSize;
  bufferLayout.attributeCount = 3;
  VertexAttribute attributes[3] = {
      {.format = Float32x4, .offset = cubePositionOffset, .shaderLocation = 0},
      {.format = Float32x4, .offset = cubeColorOffset, .shaderLocation = 1},
      {.format = Float32x2, .offset = cubeUVOffset, .shaderLocation = 2}};

  bufferLayout.attributes = attributes;

  BufferBindingLayout buf = {};
  buf.type = BufferBindingType::UNIFORM;

  BindGroupLayoutEntry bglEntry = {};
  bglEntry.binding = 0;
  bglEntry.visibility = ShaderStage::VERTEX;
  bglEntry.buffer = &buf;

  BindGroupLayoutDescriptor bglDesc = {};
  bglDesc.entryCount = 1;
  bglDesc.entries = &bglEntry;
  BindGroupLayout bindGroupLayout = CreateBindGroupLayout(device, bglDesc);

  PipelineLayoutDescriptor layout_descriptor;

  layout_descriptor.bindGroupLayoutCount = 1;
  layout_descriptor.bindGroupLayouts = &bindGroupLayout;

  /*


  struct BindingResource {
    TextureView textureView = 0;
    Sampler sampler = 0;
    Buffer buffer = 0;
    uint32_t offset = 0;
    uint32_t size = 0;
  };

  struct BindGroupEntry {
    uint32_t binding;
    BindingResource resource;
  };

  struct BindGroupDescriptor {
    BindGroupLayout layout;
    uint32_t entryCount;
    BindGroupEntry entries;
  };

  */

  BindGroupEntry bgEntry;
  bgEntry.binding = 0;
  bgEntry.resource = {.buffer = uniformBuffer};

  BindGroupDescriptor bindGroupDescriptor;
  bindGroupDescriptor.layout = bindGroupLayout;
  bindGroupDescriptor.entryCount = 1;
  bindGroupDescriptor.entries = &bgEntry;
  bindGroup = CreateBindGroup(device, bindGroupDescriptor);

  PipelineLayout pipeline_layout =
      CreatePipelineLayout(device, layout_descriptor);

  RenderPipelineDescriptor info = {
      .layout = pipeline_layout,
      .vertex = {.module = CreateShaderModule(device, vertexCode),
                 .entryPoint = "main",
                 .buffersCount = 1,
                 .buffers = &bufferLayout},
      .primitive = {.topology = GPUPrimitiveTopology::TRIANGLE_LIST},
      .fragment = {.module = CreateShaderModule(device, fragmentCode),
                   .entryPoint = "main",
                   .targetCount = 1,
                   .targets = &target}};
  pipeline = CreateRenderPipeline(device, info);

  ReleaseBindGroupLayout(bindGroupLayout);
}

void render_loop(size_t dt) {
  CommandEncoder encoder = CreateCommandEncoder(device);

  TextureView textureView = SwapChainGetCurrentTextureView();

  RenderPassColorAttachment colorAttachment;
  colorAttachment.view = textureView;
  colorAttachment.clearValue = {0.0f, 0.0f, 0.0, 1.0};
  colorAttachment.operations = {LoadOp::CLEAR, StoreOp::STORE};

  RenderPass pass = CommandEncoderBeginRenderPass(
      encoder, (RenderPassDescriptor){
                   .colorAttachmentsCount = 1,
                   .colorAttachments = &colorAttachment,
               });
  RenderPassEncoderSetPipeline(pass, pipeline);
  RenderPassEncoderSetBindGroup(pass, 0, bindGroup, 0, nullptr);
  RenderPassEncoderSetVertexBuffer(pass, 0, vtxBuffer, 0, cubeVertexArraySize);
  RenderPassEncoderDraw(pass, cubeVertexCount);
  RenderPassEncoderEnd(pass);
  CommandBuffer finish = CommandEncoderFinish(encoder, nullptr);
  QueueSubmit(queue, 1, finish);

  CommandBufferRelease(finish);
  CommandEncoderRelease(encoder);
  TextureViewRelease(textureView);
  RenderPassEncoderRelease(pass);
};

extern "C" auto wasm_main() -> void {

  if (device = GetDevice(); device == 0) {
    puts("Error opening device.");
    return;
  }
  create_arena(&transfer_arena, 64 * 1024 * 1024);

  ShaderModule fragmentShader = CreateShaderModule(device, fragmentCode);
  ShaderModule vertexShader = CreateShaderModule(device, vertexCode);

  vtxBuffer = CreateBuffer(
      device,
      {.size = cubeVertexArraySize,
       .usage = (BufferUsage)(BufferUsage::VERTEX | BufferUsage::COPY_DST),
       .mappedAtCreation = true});

  uniformBuffer = CreateBuffer(
      device,
      {.size = sizeof(Math::mat4),
       .usage = (BufferUsage)(BufferUsage::UNIFORM | BufferUsage::COPY_DST),
       .mappedAtCreation = false});

  void *data = BufferGetMappedRange(vtxBuffer, 0, cubeVertexArraySize);

  memcpy(data, (void *)cubeVertexArray, cubeVertexArraySize);

  BufferUnmap(vtxBuffer);

  build_pipeline();

  queue = DeviceGetQueue(device);
#ifdef __wasm__
  request_animation_frame((void *)render_loop);
#endif

#ifndef __wasm__
  DestroyBuffer(vtxBuffer);
#endif
}
