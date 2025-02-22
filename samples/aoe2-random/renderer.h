#pragma once

#include <arena.hpp>
#include <string.hpp>
#include <webgpu.hpp>

#include <math/matrix.hpp>
#include <promise.hpp>
#include <std/stdio.hpp>

#include "primitives/cube.hpp"
#include "primitives/cylinder.hpp"
#include "primitives/quad.hpp"

const char *vertexCode =
    /* wgsl */
    R"(
struct Uniforms {
  viewProj : mat4x4f,
  time: f32,
  civ_uv: vec2f
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


fn adjugate( m: mat4x4f ) -> mat3x3f
{
    return mat3x3f(cross(m[1].xyz, m[2].xyz), 
                cross(m[2].xyz, m[0].xyz), 
                cross(m[0].xyz, m[1].xyz));
}
@vertex fn main( vtx: VertexIn ) -> VertexOut {
  var vsOut: VertexOut;
  vsOut.Position = uniforms.viewProj * vec4f(vtx.position, 1.0);
  vsOut.fragPos = vsOut.Position.xyz;
  var uv: vec2f = vec2f(1.0-vtx.uv.x, vtx.uv.y);
  vsOut.uv = (uv * (1.0/9.0)) + uniforms.civ_uv;
  vsOut.normal = normalize(adjugate(uniforms.viewProj) * vtx.normal);
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

  fn bump(uv: vec2f) -> vec2f {
    var s: vec2f = vec2f(1.0 / 4096.0);//textureSize(myTexture);
    var x0: f32 = textureSample(myTexture, mySampler, uv + vec2f(s.x, 0.0)).r;
    var x1: f32 = textureSample(myTexture, mySampler, uv - vec2f(s.x, 0.0)).r;
    var y0: f32 = textureSample(myTexture, mySampler, uv + vec2f(0.0, s.y)).r;
    var y1: f32 = textureSample(myTexture, mySampler, uv - vec2f(0.0, s.y)).r;

    return vec2f(x0-x1, y0-y1);
  }

  @fragment fn main(fsInput: FragmentIn) -> @location(0) vec4f {
  var albedo: vec4f = textureSample(myTexture, mySampler, fsInput.uv);
  var L: vec3f = normalize(vec3f(0.0, -0.5, -6.0));

  var B: vec2f = bump(fsInput.uv);
  var N: vec3f = normalize(fsInput.normal + vec3f(0.0, B * 5.0));
  var V: vec3f = normalize(fsInput.fragPos - vec3f(0.0, 0.0, 6.0));
  var H: vec3f = normalize(L + V);
  var spec: vec3f = pow(max(dot(N, H), 0.0), albedo.r *  32.0) * vec3f(1.0) * 2.0;
  var NdotL: f32 = max(0.0, dot(L, N));
  
  //return(vec4f(N, 1.0));
  return vec4f(((NdotL + spec) * albedo.rgb) * albedo.a, albedo.a);
})";

using namespace wgpu;

Pipeline pipeline{};
Queue queue{};
Buffer vtxBuffer{};
Buffer idxBuffer{};
Buffer uniformBuffer{};
BindGroup bindGroup[3];
Texture depthBuffer{};
Texture cubeTexture{};

struct Object {
  Math::mat4 transform;
  Buffer vtxBuffer;
  Buffer idxBuffer;
  Buffer uniformBuffer;
  BindGroup bindGroup;
  uint32_t uniform_offset;
  uint32_t vtx_count;
  uint32_t idx_count;
};

struct UniformBuffer {
  Math::mat4 viewProj;
  float time;
  float padding[1]; // padding <-- VERY IMPORTANT
  Math::vec2 civ_uv;
};

struct RenderState {
  Device device{};
  CommandEncoder encoder;
  TextureView textureView;
  TextureView depthView;
};

struct UniformData {
  Math::mat4 transform;
  Math::vec2 uv;
  Object renderObject;
};

RenderState state;

void build_pipeline() {

  ColorTargetState target;
  target.format = GetPreferredCanvasFormat();

  VertexBufferLayout bufferLayout;
  bufferLayout.arrayStride = sizeof(Math::Vertex);
  bufferLayout.attributeCount = 3;
  VertexAttribute attributes[3] = {{.format = Float32x3,
                                    .offset = offsetof(Math::Vertex, position),
                                    .shaderLocation = 0},
                                   {.format = Float32x2,
                                    .offset = offsetof(Math::Vertex, texCoord),
                                    .shaderLocation = 1},
                                   {.format = Float32x3,
                                    .offset = offsetof(Math::Vertex, normal),
                                    .shaderLocation = 2}};

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
  BindGroupLayout bindGroupLayout =
      CreateBindGroupLayout(state.device, bglDesc);

  PipelineLayoutDescriptor layout_descriptor;

  layout_descriptor.bindGroupLayoutCount = 1;
  layout_descriptor.bindGroupLayouts = &bindGroupLayout;

  SamplerDescriptor samplerDesc = {
      .address_mode_u = REPEAT,
      .address_mode_v = REPEAT,
      .mag_filter = FilterMode::LINEAR,
      .min_filter = FilterMode::LINEAR,
  };

  Sampler sampler = CreateSampler(state.device, samplerDesc);

  BindGroupEntry bgEntries0[3] = {
      {.binding = 0,
       .resource = {.buffer = uniformBuffer,
                    .offset = 0,
                    .size = sizeof(UniformBuffer)}},
      {.binding = 1, .resource = {.sampler = sampler}},
      {.binding = 2,
       .resource = {.textureView = CreateTextureView(cubeTexture)}}};

  BindGroupDescriptor bindGroupDescriptor;
  bindGroupDescriptor.layout = bindGroupLayout;
  bindGroupDescriptor.entryCount = 3;
  bindGroupDescriptor.entries = bgEntries0;
  bindGroup[0] = CreateBindGroup(state.device, bindGroupDescriptor);

  BindGroupEntry bgEntries1[3] = {
      {.binding = 0,
       .resource = {.buffer = uniformBuffer,
                    .offset = 256,
                    .size = sizeof(UniformBuffer)}},
      {.binding = 1, .resource = {.sampler = sampler}},
      {.binding = 2,
       .resource = {.textureView = CreateTextureView(cubeTexture)}}};

  bindGroupDescriptor.entries = bgEntries1;
  bindGroup[1] = CreateBindGroup(state.device, bindGroupDescriptor);

  BindGroupEntry bgEntries2[3] = {
      {.binding = 0,
       .resource = {.buffer = uniformBuffer,
                    .offset = 512,
                    .size = sizeof(UniformBuffer)}},
      {.binding = 1, .resource = {.sampler = sampler}},
      {.binding = 2,
       .resource = {.textureView = CreateTextureView(cubeTexture)}}};

  bindGroupDescriptor.entries = bgEntries2;
  bindGroup[2] = CreateBindGroup(state.device, bindGroupDescriptor);

  PipelineLayout pipeline_layout =
      CreatePipelineLayout(state.device, layout_descriptor);

  RenderPipelineDescriptor info = {
      .layout = pipeline_layout,
      .vertex = {.module = CreateShaderModule(state.device, vertexCode),
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
      .fragment = {.module = CreateShaderModule(state.device, fragmentCode),
                   .entryPoint = "main",
                   .targetCount = 1,
                   .targets = &target},
  };
  pipeline = CreateRenderPipeline(state.device, info);

  depthBuffer = CreateTexture(
      state.device,
      {
          .size = {(int)getWindowWidth(), (int)getWindowHeight(), 0},
          .dimension = TextureDimension::d2D,
          .format = TextureFormat::Depth24Plus,
          .usage = TextureUsage::RENDER_ATTACHMENT,
      });

  ReleaseBindGroupLayout(bindGroupLayout);
}

float time = 0.0f;

RenderPass RenderBegin() {
  state.encoder = CreateCommandEncoder(state.device);

  state.textureView = SwapChainGetCurrentTextureView();
  state.depthView = CreateTextureView(depthBuffer);

  RenderPassColorAttachment colorAttachment;
  colorAttachment.view = state.textureView;
  colorAttachment.clearValue = {0.0f, 0.0f, 0.0f, 0.0};
  colorAttachment.operations = {LoadOp::CLEAR, StoreOp::STORE};

  RenderPassDepthStencilAttachment depthAttachment;
  depthAttachment.view = state.depthView;
  depthAttachment.clearValue = {1.0f, 1.0f};
  depthAttachment.depthOps = {LoadOp::CLEAR, StoreOp::STORE};

  RenderPass pass = CommandEncoderBeginRenderPass(
      state.encoder,
      (RenderPassDescriptor){.colorAttachmentsCount = 1,
                             .colorAttachments = &colorAttachment,
                             .depthStencilAttachmentsCount = 1,
                             .depthStencilAttachments = &depthAttachment});
  return pass;
}

void RenderEnd(RenderPass pass) {

  RenderPassEncoderEnd(pass);
  RenderPassEncoderRelease(pass);
  CommandBuffer finish = CommandEncoderFinish(state.encoder, nullptr);
  CommandEncoderRelease(state.encoder);
  QueueSubmit(queue, 1, finish);

  CommandBufferRelease(finish);
  TextureViewRelease(state.textureView);
  TextureViewRelease(state.depthView);
}

void Draw(RenderPass pass, const UniformData &data) {

  UniformBuffer uBuffer = {
      .viewProj = data.transform, .time = time, .civ_uv = data.uv};

  QueueWriteBuffer(queue, data.renderObject.uniformBuffer,
                   data.renderObject.uniform_offset, (char *)&uBuffer,
                   sizeof(UniformBuffer));

  RenderPassEncoderSetPipeline(pass, pipeline);
  RenderPassEncoderSetVertexBuffer(pass, 0, data.renderObject.vtxBuffer, 0,
                                   data.renderObject.vtx_count *
                                       sizeof(Math::Vertex));
  RenderPassEncoderSetIndexBuffer(pass, data.renderObject.idxBuffer, 0, 0,
                                  data.renderObject.idx_count *
                                      sizeof(uint32_t));

  RenderPassEncoderSetBindGroup(pass, 0, data.renderObject.bindGroup, 0,
                                nullptr);
  RenderPassEncoderDrawIndexed(pass, data.renderObject.idx_count);
}
