#pragma once

#include "defs.hpp"
#include "string.hpp"

namespace wgpu {

struct Device;
struct Queue;
struct TextureView;
struct Texture;

struct RenderTarget {
  size_t format;
};

struct PipelineInfo {
  size_t layout;
  size_t vertex;
  size_t fragment;
  RenderTarget *targets;
  size_t targetCount;
  size_t primitiveTopology;
};

struct RenderPassDescriptor {
  size_t textureView;
  size_t clearValue[4];
  size_t loadOp;
  size_t storeOp;
};

namespace imports {
extern "C" {
size_t wgpuGetDevice();
size_t wgpuCreateShaderModule(size_t device, JsString);
size_t wgpuGetPreferredCanvasFormat();
size_t wgpuCreateRenderPipeline(size_t, PipelineInfo);
size_t wgpuCreateCommandEncoder(size_t);
size_t wgpuSwapChainGetCurrentTextureView();
size_t wgpuCommandEncoderBeginRenderPass(size_t, RenderPassDescriptor);
void wgpuCommandEncoderSetPipeline(size_t, size_t);
void wgpuCommandEncoderDraw(size_t, size_t);
void wgpuCommandEncoderEnd(size_t);
size_t wgpuDeviceGetQueue(size_t);
size_t wgpuCommandEncoderFinish(size_t);
void wgpuQueueSubmit(size_t, size_t, size_t);
void wgpuCommandEncoderRelease(size_t);
void wgpuTextureViewRelease(size_t);
void wgpuRenderPassEncoderRelease(size_t);
}
} // namespace imports

static size_t GetDevice() { return imports::wgpuGetDevice(); }
static size_t CreateShaderModule(size_t device, JsString code) {
  return imports::wgpuCreateShaderModule(device, code);
}
static size_t GetPreferredCanvasFormat() {
  return imports::wgpuGetPreferredCanvasFormat();
}
static size_t CreateRenderPipeline(size_t device, PipelineInfo info) {
  return imports::wgpuCreateRenderPipeline(device, info);
}

static size_t DeviceGetQueue(size_t device) {
  return imports::wgpuDeviceGetQueue(device);
}
static size_t CreateCommandEncoder(size_t device) {
  return imports::wgpuCreateCommandEncoder(device);
}
static size_t SwapChainGetCurrentTextureView() {
  return imports::wgpuSwapChainGetCurrentTextureView();
}
static size_t CommandEncoderBeginRenderPass(size_t encoder,
                                            RenderPassDescriptor descriptor) {
  return imports::wgpuCommandEncoderBeginRenderPass(encoder, descriptor);
}
static void CommandEncoderSetPipeline(size_t encoder, size_t pipeline) {
  imports::wgpuCommandEncoderSetPipeline(encoder, pipeline);
}
static void CommandEncoderDraw(size_t encoder, size_t count) {
  imports::wgpuCommandEncoderDraw(encoder, count);
}
static void CommandEncoderEnd(size_t encoder) {
  imports::wgpuCommandEncoderEnd(encoder);
}
static size_t CommandEncoderFinish(size_t encoder, void *) {
  return imports::wgpuCommandEncoderFinish(encoder);
}

static void QueueSubmit(size_t queue, size_t count, size_t commands) {
  imports::wgpuQueueSubmit(queue, count, commands);
}

static void TextureViewRelease(size_t texture_view) {
  imports::wgpuTextureViewRelease(texture_view);
}
static void CommandEncoderRelease(size_t encoder) {
  imports::wgpuCommandEncoderRelease(encoder);
}

static void RenderPassEncoderRelease(size_t renderPass) {
  imports::wgpuRenderPassEncoderRelease(renderPass);
}

} // namespace wgpu