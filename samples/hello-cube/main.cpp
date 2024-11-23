#include "cube.hpp"
#include <arena.hpp>
#include <string.hpp>
#include <webgpu.hpp>

struct TextureSize {
  int x, y, z;
};

extern "C" void request_animation_frame(void *);
extern "C" float trunc(float);

extern "C" {
Arena transfer_arena;
}

float fmodf(float x, float y) { return x - trunc(x / y) * y; }

float min(float a, float b) {
  if (a > b)
    return a;
  return b;
}

float max(float a, float b) {
  if (a > b)
    return b;
  return a;
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

void build_pipeline() {
  ColorTargetState target;
  target.format = GetPreferredCanvasFormat();

  VertexBufferLayout bufferLayout;
  bufferLayout.arrayStride = cubeVertexSize;
  bufferLayout.attributeCount = 3;
  VertexAttribute attributes[3] = {
      {.format = Float32x4, .offset = cubePositionOffset, .shaderLocation = 0},
      {.format = Float32x4, .offset = cubeColorOffset,       .shaderLocation = 1},
      {.format = Float32x2, .offset = cubeUVOffset,       .shaderLocation = 2}};

  bufferLayout.attributes = attributes;

  RenderPipelineDescriptor info = {
      .layout = {},
      .vertex = {.module = CreateShaderModule(device, vertexCode),
                 .entryPoint = JsString("main"),
                 .buffersCount = 1,
                 .buffers = &bufferLayout},
      .primitive = {.topology = GPUPrimitiveTopology::TRIANGLE_LIST},
      .fragment = {.module = CreateShaderModule(device, fragmentCode),
                   .entryPoint = JsString("main"),
                   .targetCount = 1,
                   .targets = &target}};
  pipeline = CreateRenderPipeline(device, info);
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

  void *data = BufferGetMappedRange(vtxBuffer, 0, cubeVertexArraySize);

  memcpy(data, (void*)cubeVertexArray, cubeVertexArraySize);

  BufferUnmap(vtxBuffer);

  build_pipeline();

  queue = DeviceGetQueue(device);

  request_animation_frame((void *)render_loop);

  // DestroyBuffer(vtxBuffer);
}
