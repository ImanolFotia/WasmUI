#pragma once

#include "defs.hpp"
#include "string.hpp"

#include <stdint.h>

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

//#pragma pack(push, 1)
struct RenderTarget {
  uint32_t format;
};

struct VertexAttribute {
  VertexFormat format;
  uint32_t offset;
  uint32_t shaderLocation;
};

struct VertexBufferLayout {
  uint32_t arrayStride;
  uint32_t attributeCount;
  VertexAttribute *attributes;
};

struct VertexState {
  ShaderModule module;
  JsString entryPoint;
  uint32_t buffersCount;
  VertexBufferLayout *buffers;
};

struct BlendComponent {
  uint32_t srcFactor;
  uint32_t dstFactor;
  uint32_t blendOperation;
};


struct BlendState {
  BlendComponent color;
  BlendComponent alpha;
};

struct ColorTargetState {
  uint32_t format;
  BlendState blend;
  uint32_t writeMask;
};


struct FragmentState {
  ShaderModule module;
  JsString entryPoint;
  uint32_t targetCount;
  ColorTargetState *targets;
};

struct BufferBindingLayout {
  uint32_t type;
};

struct BindGroupLayoutEntry {
  uint32_t binding;
  uint32_t visibility;
  uint32_t type;
  uint32_t count;
  // BufferBindingLayout buffer;
};

struct PushConstantRange {
  uint32_t shaderStages;
  uint32_t range[2];
};

struct BindGroupLayoutDescriptor {
  ShaderModule module;
  JsString entryPoint;
  uint32_t entryCount;
  BindGroupLayoutEntry *entries;
};


struct PipelineLayoutDescriptor {
  uint32_t bindGroupLayoutCount;
  BindGroupLayout *bindGroupLayouts;
  uint32_t pushConstantRangesCount;
  PushConstantRange *pushConstantRanges;
};

struct PrimitiveState {
  uint32_t topology;
  uint32_t stripIndexFormat;
  uint32_t frontFace;
  uint32_t cullMode;
  bool unclippedDepth;
  uint32_t polygonMode;
  bool conservative;
};

struct StencilFaceState {
  uint32_t compare;
  uint32_t failOp;
  uint32_t stencilFailOp;
  uint32_t passOp;
};

struct StencilState {
  StencilFaceState front;
  StencilFaceState back;
  uint32_t readMask;
  uint32_t writeMask;
};

struct DepthBiasState {
  uint32_t constant;
  float slope_scale;
  float clamp;
};

struct DepthStencilState {
  uint32_t format;
  bool depthWriteEnabled;
  uint32_t depthCompare;
  StencilState stencil;
  DepthBiasState bias;
};

struct MultiSampleState {
  uint32_t count;
  uint32_t mask;
  bool alphaToCoverageEnabled;
};


struct RenderPipelineDescriptor {
  PipelineLayout layout;
  VertexState vertex;
  PrimitiveState primitive;
  DepthStencilState depthStencil;
  MultiSampleState multisample;
  FragmentState fragment;
  uint32_t miltiview;
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
  TextureView resolveTarget;
  Operations operations;
  Color clearValue;
};

struct RenderPassDepthStencilAttachment {
  TextureView *view;
  uint32_t depthOpsCount;
  Operations depthOps;
  uint32_t stencilOpsCount;
  Operations stencilOps;
};

struct RenderPassTimestampWrites {
  QuerySet *querySet;
  uint32_t beginningOfPassWriteIndex;
  uint32_t endOfPassWriteIndex;
};

struct RenderPassDescriptor {
  uint32_t colorAttachmentsCount;
  RenderPassColorAttachment *colorAttachments;
  uint32_t depthStencilAttachmentsCount = 0;
  RenderPassDepthStencilAttachment *depthStencilAttachments = nullptr;
  RenderPassTimestampWrites timestampWrites;
  QuerySet *occlusionQuerySet;
};

struct BufferDescriptor {
  uint32_t size{};
  BufferUsage usage{};
  bool mappedAtCreation{};
};


//#pragma pack(pop)

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
void wgpuRenderPassEncoderDraw(RenderPass, uint32_t);
void wgpuRenderPassEncoderEnd(RenderPass);
Queue wgpuDeviceGetQueue(Device);
CommandBuffer wgpuCommandEncoderFinish(CommandEncoder);
void wgpuQueueSubmit(Queue, uint32_t, CommandBuffer);
void wgpuCommandEncoderRelease(CommandEncoder);
void wgpuTextureViewRelease(TextureView);
void wgpuRenderPassEncoderRelease(RenderPass);
void wgpuCommandBufferRelease(CommandBuffer);
Buffer wgpuCreateBuffer(Device, BufferDescriptor);
void wgpuRenderPassEncoderSetVertexBuffer(RenderPass, uint32_t, Buffer, uint32_t,
                                          uint32_t);
void wgpuDestroyBuffer(Buffer);

void *wgpuBufferGetMappedRange(Buffer, uint32_t, uint32_t);
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
static void RenderPassEncoderDraw(RenderPass encoder, uint32_t count) {
  imports::wgpuRenderPassEncoderDraw(encoder, count);
}
static void RenderPassEncoderEnd(RenderPass encoder) {
  imports::wgpuRenderPassEncoderEnd(encoder);
}
static CommandBuffer CommandEncoderFinish(CommandEncoder encoder, void *) {
  return imports::wgpuCommandEncoderFinish(encoder);
}

static void QueueSubmit(Queue queue, uint32_t count, CommandBuffer commands) {
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

static void RenderPassEncoderSetVertexBuffer(RenderPass encoder, uint32_t slot,
                                             Buffer buffer, uint32_t offset,
                                             uint32_t size) {
  imports::wgpuRenderPassEncoderSetVertexBuffer(encoder, slot, buffer, offset,
                                                size);
}

static void *BufferGetMappedRange(Buffer buffer, uint32_t offset, uint32_t size) {
  return imports::wgpuBufferGetMappedRange(buffer, offset, size);
}

static void BufferUnmap(Buffer buffer) { imports::wgpuBufferUnmap(buffer); }

} // namespace wgpu