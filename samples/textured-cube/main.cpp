// #include "cube.hpp"
#include <arena.hpp>
#include <string.hpp>
#include <webgpu.hpp>

#include <math/matrix.hpp>
#include <promise.hpp>
#include <std/stdio.hpp>

#include "primitives/cube.hpp"

extern "C" void request_animation_frame(void *);
extern "C" FILE fopen(JsString);
extern "C" void fread(FILE, char *, size_t);

const char *vertexCode =
    /* wgsl */
    R"(
struct Uniforms {
  viewProj : mat4x4f,
  time: f32
};
@binding(0) @group(0) var<uniform> uniforms : Uniforms;
struct VertexOut {
    @builtin(position) Position: vec4f,
    @location(0) uv: vec2f,
    @location(1) normal: vec3f,
    @location(2) time: f32,
    @location(3) fragPos: vec3f
  }; 
struct VertexIn {
  @location(0) position: vec3f,
  @location(1) uv: vec2f,
  @location(2) normal: vec3f
};
@vertex fn main( vtx: VertexIn ) -> VertexOut {
  var vsOut: VertexOut;
  vsOut.Position = uniforms.viewProj * vec4f(vtx.position, 1.0);
  vsOut.fragPos = vsOut.Position.xyz;
  vsOut.uv = vtx.uv;
  vsOut.normal = vtx.normal;
  vsOut.time = uniforms.time;
  return vsOut;
})";

const char *fragmentCode =
    R"(
@group(0) @binding(1) var mySampler: sampler;
@group(0) @binding(2) var myTexture: texture_2d<f32>;
 struct FragmentIn {
    @location(0) uv: vec2f,
    @location(1) normal: vec3f,
    @location(2) time: f32,
    @location(3) fragPos: vec3f
  }; 
  @fragment fn main(fsInput: FragmentIn) -> @location(0) vec4f {
  var albedo: vec3f = textureSample(myTexture, mySampler, fsInput.uv).rgb;
  var time: f32 = fsInput.time;
  var lightPos: vec3f = vec3f(sin(time), 1.0, -cos(time))*3;
  var dist : f32 = distance(fsInput.fragPos,lightPos);
  var light: f32 = 10.0 * (1.0/(dist*dist)) * max(0.0, dot(lightPos, fsInput.normal));
  var fragColor: vec3f = (albedo * light * vec3f(1.0, 0.7, 0.38)) + (albedo * 0.1);
  return vec4f(fragColor, 1.0);
})";

using namespace wgpu;

Device device{};
Pipeline pipeline{};
Queue queue{};
Buffer vtxBuffer{};
Buffer idxBuffer{};
Buffer uniformBuffer{};
BindGroup bindGroup{};
Texture depthBuffer{};
Texture cubeTexture{};

struct UniformBuffer {
  Math::mat4 viewProj;
  float time;
  float padding[3];
};

size_t cube_vertex_size = 0;
size_t cube_index_size = 0;
size_t indexCount = 0;

void build_pipeline() {

  ColorTargetState target;
  target.format = GetPreferredCanvasFormat();

  VertexBufferLayout bufferLayout;
  bufferLayout.arrayStride = sizeof(Math::Vertex);
  bufferLayout.attributeCount = 3;
  VertexAttribute attributes[3] = {
      {.format = Float32x3, .offset = offsetof(Math::Vertex, position), .shaderLocation = 0},
      {.format = Float32x2, .offset = offsetof(Math::Vertex, texCoord), .shaderLocation = 1},
      {.format = Float32x3, .offset = offsetof(Math::Vertex, normal), .shaderLocation = 2}};

  bufferLayout.attributes = attributes;

  BufferBindingLayout buf = {};
  buf.type = BufferBindingType::UNIFORM;

  SamplerBindingLayout samp;
  samp.type = SamplerBindingType::FILTERING;

  TextureBindingLayout tex = {.sampleType = TextureSampleType::FLOAT,
                              .viewDimension = TextureViewDimension::d2D,
                              .multisampled = false};

  BindGroupLayoutEntry bglEntries[3] = {{
                                            .binding = 0,
                                            .visibility = ShaderStage::VERTEX,
                                            .buffer = &buf,
                                        },
                                        {
                                            .binding = 1,
                                            .visibility = ShaderStage::FRAGMENT,
                                            .sampler = &samp,
                                        },
                                        {
                                            .binding = 2,
                                            .visibility = ShaderStage::FRAGMENT,
                                            .texture = &tex,
                                        }};

  BindGroupLayoutDescriptor bglDesc = {};
  bglDesc.entryCount = 3;
  bglDesc.entries = bglEntries;
  BindGroupLayout bindGroupLayout = CreateBindGroupLayout(device, bglDesc);

  PipelineLayoutDescriptor layout_descriptor;

  layout_descriptor.bindGroupLayoutCount = 1;
  layout_descriptor.bindGroupLayouts = &bindGroupLayout;

  SamplerDescriptor samplerDesc = {
      .address_mode_u = REPEAT,
      .address_mode_v = REPEAT,
      .mag_filter = FilterMode::LINEAR,
      .min_filter = FilterMode::LINEAR,
  };

  Sampler sampler = CreateSampler(device, samplerDesc);

  BindGroupEntry bgEntries[3] = {
      {.binding = 0, .resource = {.buffer = uniformBuffer, .size = 80}},
      {.binding = 1, .resource = {.sampler = sampler}},
      {.binding = 2,
       .resource = {.textureView = CreateTextureView(cubeTexture)}}};

  BindGroupDescriptor bindGroupDescriptor;
  bindGroupDescriptor.layout = bindGroupLayout;
  bindGroupDescriptor.entryCount = 3;
  bindGroupDescriptor.entries = bgEntries;
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
      .depthStencil =
          {
              .format = TextureFormat::Depth24Plus,
              .depthWriteEnabled = true,
              .depthCompare = CompareFunction::Less,
          },
      .fragment = {.module = CreateShaderModule(device, fragmentCode),
                   .entryPoint = "main",
                   .targetCount = 1,
                   .targets = &target},
  };
  pipeline = CreateRenderPipeline(device, info);

  depthBuffer = CreateTexture(
      device, {
                  .size = {(int)getWindowWidth(), (int)getWindowHeight(), 0},
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

  TextureView textureView = SwapChainGetCurrentTextureView();
  TextureView depthView = CreateTextureView(depthBuffer);

  RenderPassColorAttachment colorAttachment;
  colorAttachment.view = textureView;
  colorAttachment.clearValue = {0.094117f, 0.094117f, 0.094117, 1.0};
  colorAttachment.operations = {LoadOp::CLEAR, StoreOp::STORE};

  RenderPassDepthStencilAttachment depthAttachment;
  depthAttachment.view = depthView;
  depthAttachment.clearValue = {1.0f, 1.0f};
  depthAttachment.depthOps = {LoadOp::CLEAR, StoreOp::STORE};

  RenderPass pass = CommandEncoderBeginRenderPass(
      encoder,
      (RenderPassDescriptor){.colorAttachmentsCount = 1,
                             .colorAttachments = &colorAttachment,
                             .depthStencilAttachmentsCount = 1,
                             .depthStencilAttachments = &depthAttachment});
  using Math::mat4;
  using Math::vec3;

  uint32_t width = getWindowWidth();
  uint32_t height = getWindowHeight();

  mat4 view = Math::lookAt(
      vec3(Math::sin(time * 0.5) * 6.0f, 3.0f, Math::cos(time * 0.5) * 6.0f),
      vec3(0.0f, 0.0, 0.0f), vec3(0.0f, 1.0f, 0.0f));

  mat4 proj = Math::perspective(Math::radians(45.0f), width / (height + 0.001),
                                0.1f, 100.0f);

  mat4 transform = proj * view;
  UniformBuffer uBuffer{.viewProj = transform, .time = time};
  QueueWriteBuffer(queue, uniformBuffer, 0, (char *)&uBuffer, 80);

  RenderPassEncoderSetPipeline(pass, pipeline);
  RenderPassEncoderSetBindGroup(pass, 0, bindGroup, 0, nullptr);
  RenderPassEncoderSetVertexBuffer(pass, 0, vtxBuffer, 0, cube_vertex_size);
  RenderPassEncoderSetIndexBuffer(pass, idxBuffer, 0, 0, cube_index_size);
  RenderPassEncoderDrawIndexed(pass, indexCount);
  RenderPassEncoderEnd(pass);
  RenderPassEncoderRelease(pass);
  CommandBuffer finish = CommandEncoderFinish(encoder, nullptr);
  CommandEncoderRelease(encoder);
  QueueSubmit(queue, 1, finish);

  CommandBufferRelease(finish);
  TextureViewRelease(textureView);
  TextureViewRelease(depthView);
};

int main(int argc, char **argv) {

  if (device = GetDevice(); device == 0) {
    puts("Error opening device.");
    return 1;
  }

  queue = DeviceGetQueue(device);

  ShaderModule fragmentShader = CreateShaderModule(device, fragmentCode);
  ShaderModule vertexShader = CreateShaderModule(device, vertexCode);

  engine::Cube cube;
  indexCount = cube.data().num_indices;
  cube_vertex_size = cube.data().num_vertices * sizeof(Math::Vertex);
  cube_index_size = indexCount * sizeof(uint32_t);

  vtxBuffer = CreateBuffer(device, {.size = cube_vertex_size,
                                    .usage = (BufferUsage)(BufferUsage::VERTEX),
                                    .mappedAtCreation = true});

  idxBuffer = CreateBuffer(device, {.size = cube_index_size,
                                    .usage = (BufferUsage)(BufferUsage::INDEX),
                                    .mappedAtCreation = true});

  {
    void *data = BufferGetMappedRange(vtxBuffer, 0, cube_vertex_size);
    std::memcpy(data, (void *)cube.data().Vertices, cube_vertex_size);
  }
    BufferUnmap(vtxBuffer);
  {
    void *data = BufferGetMappedRange(idxBuffer, 0, cube_index_size);
    std::memcpy(data, (void *)cube.data().Indices, cube_index_size);
  }
    BufferUnmap(idxBuffer);
  uniformBuffer = CreateBuffer(
      device,
      {.size = 80,
       .usage = (BufferUsage)(BufferUsage::UNIFORM | BufferUsage::COPY_DST),
       .mappedAtCreation = false});


  cubeTexture =
      CreateTexture(device, {
                                .size = {512, 512, 0},
                                .dimension = TextureDimension::d2D,
                                .format = TextureFormat::Rgba8Unorm,
                                .usage = TextureUsage::RENDER_ATTACHMENT |
                                         TextureUsage::COPY_DST |
                                         TextureUsage::TEXTURE_BINDING,
                            });

  fetch("../../media/diff.png").then([](Response response) {
    response.blob().then([](Blob blob) {
      createImageBitmap(blob.handle).then([](Image image) {
        QueueCopyExternalImageToTexture(queue, (void *)image.handle,
                                        cubeTexture, {512, 512, 0});
      });
    });
  });

  build_pipeline();

  std::printf("%, %! % %", "Hello", "World", 654, 87.14);

#ifdef __wasm__
  request_animation_frame((void *)render_loop);
#endif

#ifndef __wasm__
  DestroyBuffer(vtxBuffer);
#endif
}
