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
CREATE_HANDLE(RenderPass);
CREATE_HANDLE(CommandEncoder);
CREATE_HANDLE(CommandBuffer);
CREATE_HANDLE(ShaderModule);
CREATE_HANDLE(Buffer);

struct RenderTarget {
  size_t format;
};

struct PipelineLayout {

};

struct VertexStage {
  
}

struct PipelineDescriptor {
  PipelineLayout layout;
  ShaderModule vertex;
  ShaderModule fragment;
  RenderTarget *targets;
  size_t targetCount;
  size_t primitiveTopology;
  size_t depthStencil[2];
};

struct RenderPassDescriptor {
  TextureView textureView;
  size_t clearValue[4];
  size_t loadOp;
  size_t storeOp;
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
Pipeline wgpuCreateRenderPipeline(Device, PipelineDescriptor);
CommandEncoder wgpuCreateCommandEncoder(Device);
TextureView wgpuSwapChainGetCurrentTextureView();
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
static Pipeline CreateRenderPipeline(Device device, PipelineDescriptor info) {
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