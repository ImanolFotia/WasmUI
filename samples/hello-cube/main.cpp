#include "cube.hpp"
#include <arena.hpp>
#include <string.hpp>
#include <webgpu.hpp>
#include <stdio.hpp>
#include <math/matrix.hpp>

extern "C" void request_animation_frame(void *);
extern "C" FILE fopen(JsString);
extern "C" void fread(FILE, char *, size_t);

const int sampleCount = 4;

const char *vertexCode =
    R"(
    struct Uniforms {
      viewProj : mat4x4f,
    };
    @binding(0) @group(0) var<uniform> uniforms : Uniforms;
    struct VertexOut {
        @builtin(position) Position: vec4f,
        @location(0) color: vec4f,
      }; 
    struct VertexIn {
      @location(0) position: vec4f,
      @location(1) color: vec4f,
      @location(2) uv: vec2f,
      @location(3) normal: vec3f
    };
    @vertex fn main( vtx: VertexIn ) -> VertexOut {
      var vsOut: VertexOut;
      vsOut.Position = uniforms.viewProj * vtx.position;
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
BindGroup bindGroup{};
Texture depthBuffer{};
Texture swapChainTexture{};

Math::mat4 viewProj = {Math::vec4(0.936213, -0.323997, -0.325092, -0.324443),
                       Math::vec4(0.000000, 1.565988, -0.487639, -0.486664),
                       Math::vec4(-0.374485, -0.809994, -0.812731, -0.811107),
                       Math::vec4(0.000000, 0.000000, 5.976554, 6.164413)};

void build_pipeline() {

  ColorTargetState target;
  target.format = GetPreferredCanvasFormat();

  VertexBufferLayout bufferLayout;
  bufferLayout.arrayStride = cubeVertexSize;
  bufferLayout.attributeCount = 4;
  VertexAttribute attributes[4] = {
      {.format = Float32x4, .offset = cubePositionOffset, .shaderLocation = 0},
      {.format = Float32x4, .offset = cubeColorOffset, .shaderLocation = 1},
      {.format = Float32x2, .offset = cubeUVOffset, .shaderLocation = 2},
      {.format = Float32x3, .offset = cubeNormalOffset, .shaderLocation = 3}};

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

  BindGroupEntry bgEntry;
  bgEntry.binding = 0;
  bgEntry.resource = {.buffer = uniformBuffer, .size = sizeof(Math::mat4)};

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
      .depthStencil = {
        .format = TextureFormat::Depth24Plus,
        .depthWriteEnabled = true,
        .depthCompare = CompareFunction::Less,
      },
      .multisample =  {
        .count = sampleCount
      },
      .fragment = {.module = CreateShaderModule(device, fragmentCode),
                   .entryPoint = "main",
                   .targetCount = 1,
                   .targets = &target},
      };
  pipeline = CreateRenderPipeline(device, info);

  depthBuffer = CreateTexture(device, {
    .size = {(int)getWindowWidth(), (int)getWindowHeight(), 0},
                  .sampleCount = sampleCount,
    .dimension = TextureDimension::d2D,
    .format = TextureFormat::Depth24Plus,
    .usage = TextureUsage::RENDER_ATTACHMENT,
  });

  ReleaseBindGroupLayout(bindGroupLayout);
}

float time = 0.0f;
extern "C" void render_loop(float dt) {
  time += dt;
  CommandEncoder encoder = CreateCommandEncoder(device);

  TextureView textureView = CreateTextureView(swapChainTexture);// 
  TextureView depthView = CreateTextureView(depthBuffer);

  RenderPassColorAttachment colorAttachment;
  colorAttachment.view = textureView;
  colorAttachment.resolveTarget = SwapChainGetCurrentTextureView();
  colorAttachment.clearValue = {0.094117f, 0.094117f, 0.094117, 1.0};
  colorAttachment.operations = {LoadOp::CLEAR, StoreOp::STORE};

  RenderPassDepthStencilAttachment depthAttachment;
  depthAttachment.view = depthView;
  depthAttachment.clearValue = {1.0f, 1.0f};
  depthAttachment.depthOps = {LoadOp::CLEAR, StoreOp::STORE};

  RenderPass pass = CommandEncoderBeginRenderPass(
      encoder, (RenderPassDescriptor){
                   .colorAttachmentsCount = 1,
                   .colorAttachments = &colorAttachment,
                   .depthStencilAttachmentsCount = 1,
                   .depthStencilAttachments = &depthAttachment
               });
  using Math::mat4;
  using Math::vec3;
  
  uint32_t width = getWindowWidth();
  uint32_t height = getWindowHeight();

  mat4 view = Math::lookAt(
      vec3(Math::sin(time) * 6.0f, 3.0f, Math::cos(time ) * 6.0f),
      vec3(0.0f, 0.0, 0.0f), vec3(0.0f, 1.0f, 0.0f));

  mat4 proj = Math::perspective(Math::radians(45.0f), width / (height+0.001), 0.1f, 100.0f);

  mat4 transform = proj * view;
  QueueWriteBuffer(queue, uniformBuffer, 0, (char *)&transform, sizeof(mat4));

  RenderPassEncoderSetPipeline(pass, pipeline);
  RenderPassEncoderSetBindGroup(pass, 0, bindGroup, 0, nullptr);
  RenderPassEncoderSetVertexBuffer(pass, 0, vtxBuffer, 0, cubeVertexArraySize);
  RenderPassEncoderDraw(pass, cubeVertexCount);
  RenderPassEncoderEnd(pass);
  RenderPassEncoderRelease(pass);
  CommandBuffer finish = CommandEncoderFinish(encoder, nullptr);
  CommandEncoderRelease(encoder);
  QueueSubmit(queue, 1, finish);

  CommandBufferRelease(finish);
  TextureViewRelease(textureView);
  TextureViewRelease(depthView);
};

int main(int argc, char** argv) {

  if (device = GetDevice(); device == 0) {
    puts("Error opening device.");
    return 1;
  }

  ShaderModule fragmentShader = CreateShaderModule(device, fragmentCode);
  ShaderModule vertexShader = CreateShaderModule(device, vertexCode);

  vtxBuffer = CreateBuffer(
      device,
      {.size = cubeVertexArraySize,
       .usage = (BufferUsage)(BufferUsage::VERTEX),
       .mappedAtCreation = true});

  uniformBuffer = CreateBuffer(
      device,
      {.size = sizeof(Math::mat4),
       .usage = (BufferUsage)(BufferUsage::UNIFORM | BufferUsage::COPY_DST),
       .mappedAtCreation = false});

  void *data = BufferGetMappedRange(vtxBuffer, 0, cubeVertexArraySize);

  std::memcpy(data, (void *)cubeVertexArray, cubeVertexArraySize);

  BufferUnmap(vtxBuffer);

  swapChainTexture = CreateTexture(device, {
                  .size = {(int)getWindowWidth(), (int)getWindowHeight(), 0},
                  .sampleCount = sampleCount,
                  .dimension = TextureDimension::d2D,
                  .format = (TextureFormat)GetPreferredCanvasFormat(),
                  .usage = TextureUsage::RENDER_ATTACHMENT,
              });

  build_pipeline();

  queue = DeviceGetQueue(device);

#ifdef __wasm__
  request_animation_frame((void *)render_loop);
#endif

#ifndef __wasm__
  DestroyBuffer(vtxBuffer);
#endif
}
