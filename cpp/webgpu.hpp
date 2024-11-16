#pragma once

#include "defs.hpp"
#include "string.hpp"

namespace wgpu {

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
struct RenderTarget {
  size_t format;
};

struct PipelineInfo {
  size_t layout;
  ShaderModule vertex;
  ShaderModule fragment;
  RenderTarget *targets;
  size_t targetCount;
  size_t primitiveTopology;
};

struct RenderPassDescriptor {
  TextureView textureView;
  size_t clearValue[4];
  size_t loadOp;
  size_t storeOp;
};


namespace imports {
extern "C" {
Device wgpuGetDevice();
ShaderModule wgpuCreateShaderModule(Device device, JsString);
size_t wgpuGetPreferredCanvasFormat();
Pipeline wgpuCreateRenderPipeline(Device, PipelineInfo);
CommandEncoder wgpuCreateCommandEncoder(Device);
TextureView wgpuSwapChainGetCurrentTextureView();
RenderPass wgpuCommandEncoderBeginRenderPass(CommandEncoder,
                                             RenderPassDescriptor);
void wgpuCommandEncoderSetPipeline(RenderPass, Pipeline);
void wgpuCommandEncoderDraw(RenderPass, size_t);
void wgpuCommandEncoderEnd(RenderPass);
Queue wgpuDeviceGetQueue(Device);
CommandBuffer wgpuCommandEncoderFinish(CommandEncoder);
void wgpuQueueSubmit(Queue, size_t, CommandBuffer);
void wgpuCommandEncoderRelease(CommandEncoder);
void wgpuTextureViewRelease(TextureView);
void wgpuRenderPassEncoderRelease(RenderPass);
void wgpuRenderCommandBufferRelease(CommandBuffer);
}
} // namespace imports

static Device GetDevice() { return imports::wgpuGetDevice(); }
static ShaderModule CreateShaderModule(Device device, JsString code) {
  return imports::wgpuCreateShaderModule(device, code);
}
static size_t GetPreferredCanvasFormat() {
  return imports::wgpuGetPreferredCanvasFormat();
}
static Pipeline CreateRenderPipeline(Device device, PipelineInfo info) {
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
static void CommandEncoderSetPipeline(RenderPass encoder, Pipeline pipeline) {
  imports::wgpuCommandEncoderSetPipeline(encoder, pipeline);
}
static void CommandEncoderDraw(RenderPass encoder, size_t count) {
  imports::wgpuCommandEncoderDraw(encoder, count);
}
static void CommandEncoderEnd(RenderPass encoder) {
  imports::wgpuCommandEncoderEnd(encoder);
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

static void RenderCommandBufferRelease(CommandBuffer commandBuffer) {
  imports::wgpuRenderCommandBufferRelease(commandBuffer);
}

} // namespace wgpu