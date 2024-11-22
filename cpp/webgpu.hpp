#pragma once

#include "defs.hpp"
#include "string.hpp"

namespace wgpu {

#define WHOLE_SIZE (2147483647)

#define CREATE_HANDLE(object) typedef struct object##_t *object;

CREATE_HANDLE(Device);
CREATE_HANDLE(Queue);
CREATE_HANDLE(TextureView);
CREATE_HANDLE(Texture);
CREATE_HANDLE(Pipeline);
CREATE_HANDLE(PipelineLayout);
CREATE_HANDLE(RenderPass);
CREATE_HANDLE(CommandEncoder);
CREATE_HANDLE(CommandBuffer);
CREATE_HANDLE(ShaderModule);
CREATE_HANDLE(BindGroupLayout);
CREATE_HANDLE(Buffer);
CREATE_HANDLE(PipelineCache);
CREATE_HANDLE(QuerySet);

struct RenderTarget {
  size_t format;
};

struct VertexAttribute {
  size_t format;
  size_t offset;
  size_t shaderLocation;
};

struct VertexBufferLayout {
  size_t arrayStride;
  size_t attributeCount;
  VertexAttribute *attributes;
};

struct VertexState {
  ShaderModule module;
  JsString entryPoint;
  size_t layoutsCount;
  VertexBufferLayout *layouts;
};

struct BlendComponent {
  size_t srcFactor;
  size_t dstFactor;
  size_t blendOperation;
};


struct BlendState {
  BlendComponent color;
  BlendComponent alpha;
};

struct ColorTargetState {
  size_t format;
  BlendState blend;
  size_t writeMask;
};


struct FragmentState {
  ShaderModule module;
  JsString entryPoint;
  size_t targetCount;
  ColorTargetState *targets;
};

struct BufferBindingLayout {
  size_t type;
};

struct BindGroupLayoutEntry {
  size_t binding;
  size_t visibility;
  size_t type;
  size_t count;
  // BufferBindingLayout buffer;
};

struct PushConstantRange {
  size_t shaderStages;
  size_t range[2];
};

struct BindGroupLayoutDescriptor {
  ShaderModule module;
  JsString entryPoint;
  size_t entryCount;
  BindGroupLayoutEntry *entries;
};


struct PipelineLayoutDescriptor {
  size_t bindGroupLayoutCount;
  BindGroupLayout *bindGroupLayouts;
  size_t pushConstantRangesCount;
  PushConstantRange *pushConstantRanges;
};

struct PrimitiveState {
  size_t topology;
  size_t stripIndexFormat;
  size_t frontFace;
  size_t cullMode;
  bool unclippedDepth;
  size_t polygonMode;
  bool conservative;
};

struct StencilFaceState {
  size_t compare;
  size_t failOp;
  size_t stencilFailOp;
  size_t passOp;
};

struct StencilState {
  StencilFaceState front;
  StencilFaceState back;
  size_t readMask;
  size_t writeMask;
};

struct DepthBiasState {
  size_t constant;
  float slope_scale;
  float clamp;
};

struct DepthStencilState {
  size_t format;
  bool depthWriteEnabled;
  size_t depthCompare;
  StencilState stencil;
  DepthBiasState bias;
};

struct MultiSampleState {
  size_t count;
  size_t mask;
  bool alphaToCoverageEnabled;
};


struct RenderPipelineDescriptor {
  PipelineLayout layout;
  VertexState vertex;
  PrimitiveState primitive;
  DepthStencilState depthStencil;
  MultiSampleState multisample;
  FragmentState fragment;
  size_t miltiview;
  PipelineCache cache;
};

struct Operations {
  LoadOp load;
  StoreOp store;
};

struct Color {
  float v[4];
};

struct RenderPassColorAttachment {
  TextureView view;
  TextureView resolveView;
  Operations operations;
  Color clearValue;
};

struct RenderPassDepthStencilAttachment {
  TextureView *view;
  size_t depthOpsCount;
  Operations depthOps;
  size_t stencilOpsCount;
  Operations stencilOps;
};

struct RenderPassTimestampWrites {
  QuerySet *querySet;
  size_t beginningOfPassWriteIndex;
  size_t endOfPassWriteIndex;
};

struct RenderPassDescriptor {
  size_t colorAttachmentsCount;
  RenderPassColorAttachment *colorAttachments;
  size_t depthStencilAttachmentsCount;
  RenderPassDepthStencilAttachment *depthStencilAttachments;
  RenderPassTimestampWrites timestampWrites;
  QuerySet *occlusionQuerySet;
};

struct BufferDescriptor {
  size_t size{};
  BufferUsage usage{};
  bool mappedAtCreation{};
};

namespace imports {
extern "C" {
Device wgpuGetDevice();
ShaderModule wgpuCreateShaderModule(Device device, JsString);
size_t wgpuGetPreferredCanvasFormat();
Pipeline wgpuCreateRenderPipeline(Device, RenderPipelineDescriptor);
CommandEncoder wgpuCreateCommandEncoder(Device);
TextureView wgpuSwapChainGetCurrentTextureView();
PipelineLayout wgpuCreatePipelineLayout(Device, PipelineLayoutDescriptor);
RenderPass wgpuCommandEncoderBeginRenderPass(CommandEncoder,
                                             RenderPassDescriptor);
void wgpuRenderPassEncoderSetPipeline(RenderPass, Pipeline);
void wgpuRenderPassEncoderDraw(RenderPass, size_t);
void wgpuRenderPassEncoderEnd(RenderPass);
Queue wgpuDeviceGetQueue(Device);
CommandBuffer wgpuCommandEncoderFinish(CommandEncoder);
void wgpuQueueSubmit(Queue, size_t, CommandBuffer);
void wgpuCommandEncoderRelease(CommandEncoder);
void wgpuTextureViewRelease(TextureView);
void wgpuRenderPassEncoderRelease(RenderPass);
void wgpuCommandBufferRelease(CommandBuffer);
Buffer wgpuCreateBuffer(Device, BufferDescriptor);
void wgpuRenderPassEncoderSetVertexBuffer(RenderPass, size_t, Buffer, size_t,
                                          size_t);
void wgpuDestroyBuffer(Buffer);

void *wgpuBufferGetMappedRange(Buffer, size_t, size_t);
void wgpuBufferUnmap(Buffer);
}
} // namespace imports

static Device GetDevice() { return imports::wgpuGetDevice(); }
static ShaderModule CreateShaderModule(Device device, JsString code) {
  return imports::wgpuCreateShaderModule(device, code);
}
static size_t GetPreferredCanvasFormat() {
  return imports::wgpuGetPreferredCanvasFormat();
}
static Pipeline CreateRenderPipeline(Device device,
                                     RenderPipelineDescriptor info) {
  return imports::wgpuCreateRenderPipeline(device, info);
}

static Queue DeviceGetQueue(Device device) {
  return imports::wgpuDeviceGetQueue(device);
}
static CommandEncoder CreateCommandEncoder(Device device) {
  return imports::wgpuCreateCommandEncoder(device);
}
static TextureView SwapChainGetCurrentTextureView() {
  return imports::wgpuSwapChainGetCurrentTextureView();
}
static RenderPass
CommandEncoderBeginRenderPass(CommandEncoder encoder,
                              RenderPassDescriptor descriptor) {
  return imports::wgpuCommandEncoderBeginRenderPass(encoder, descriptor);
}
static void RenderPassEncoderSetPipeline(RenderPass encoder,
                                         Pipeline pipeline) {
  imports::wgpuRenderPassEncoderSetPipeline(encoder, pipeline);
}
static void RenderPassEncoderDraw(RenderPass encoder, size_t count) {
  imports::wgpuRenderPassEncoderDraw(encoder, count);
}
static void RenderPassEncoderEnd(RenderPass encoder) {
  imports::wgpuRenderPassEncoderEnd(encoder);
}
static CommandBuffer CommandEncoderFinish(CommandEncoder encoder, void *) {
  return imports::wgpuCommandEncoderFinish(encoder);
}

static void QueueSubmit(Queue queue, size_t count, CommandBuffer commands) {
  imports::wgpuQueueSubmit(queue, count, commands);
}

static void TextureViewRelease(TextureView texture_view) {
  imports::wgpuTextureViewRelease(texture_view);
}
static void CommandEncoderRelease(CommandEncoder encoder) {
  imports::wgpuCommandEncoderRelease(encoder);
}

static void RenderPassEncoderRelease(RenderPass renderPass) {
  imports::wgpuRenderPassEncoderRelease(renderPass);
}

static void CommandBufferRelease(CommandBuffer commandBuffer) {
  imports::wgpuCommandBufferRelease(commandBuffer);
}

static Buffer CreateBuffer(Device device, BufferDescriptor commandBuffer) {
  return imports::wgpuCreateBuffer(device, commandBuffer);
}

static void DestroyBuffer(Buffer buffer) { imports::wgpuDestroyBuffer(buffer); }

static void RenderPassEncoderSetVertexBuffer(RenderPass encoder, size_t slot,
                                             Buffer buffer, size_t offset,
                                             size_t size) {
  imports::wgpuRenderPassEncoderSetVertexBuffer(encoder, slot, buffer, offset,
                                                size);
}

static void *BufferGetMappedRange(Buffer buffer, size_t offset, size_t size) {
  return imports::wgpuBufferGetMappedRange(buffer, offset, size);
}

static void BufferUnmap(Buffer buffer) { imports::wgpuBufferUnmap(buffer); }

} // namespace wgpu