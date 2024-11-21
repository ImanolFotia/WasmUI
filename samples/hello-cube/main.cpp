#include "cube.hpp"
#include <arena.hpp>
#include <string.hpp>
#include <webgpu.hpp>

struct TextureSize {
  int x, y, z;
};

extern "C" void print(JsString);
extern "C" void request_animation_frame(void *);
extern "C" float trunc(float);
extern "C" void render_callback(float dt) { print("hello!"); }

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
        @builtin(position) position: vec4f,
        @location(0) color: vec4f,
      }; 
    struct VertexIn {
      @location(0) position: vec4f,
      @location(1) uv: vec2f
    };
    @vertex fn main( vtx: VertexIn ) -> VertexOut {
      var vsOut: VertexOut;
      vsOut.position = vtx.position;
      vsOut.color = 0.5 * (vtx.position + vec4(1.0, 1.0, 1.0, 1.0));
      return vsOut;
    })";

const char *fragmentCode =
    R"(
     struct FragmentIn {
        @builtin(position) position: vec4f,
        @location(0) color: vec4f,
      }; 
      @fragment fn main(fsInput: FragmentIn) -> @location(0) vec4f {
        return fsInput.color;
      })";

using namespace wgpu;

Device device{};
Pipeline pipeline{};
Queue queue{};
Buffer vtxBuffer{};

void render_loop(size_t dt) {
  CommandEncoder encoder = CreateCommandEncoder(device);

  TextureView textureView = SwapChainGetCurrentTextureView();

  RenderPass pass =
      CommandEncoderBeginRenderPass(encoder, {.textureView = textureView,
                                              .clearValue = {0, 0, 0, 1},
                                              .loadOp = LoadOp::CLEAR,
                                              .storeOp = StoreOp::STORE});
  RenderPassEncoderSetPipeline(pass, pipeline);
  RenderPassEncoderSetVertexBuffer(pass, 0, vtxBuffer, 0, cubeVertexArraySize);
  RenderPassEncoderDraw(pass, 3);
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
    print("Error opening device.");
    return;
  }
  create_arena(&transfer_arena, 64*1024*1024);

  ShaderModule fragmentShader = CreateShaderModule(device, fragmentCode);
  ShaderModule vertexShader = CreateShaderModule(device, vertexCode);
  auto preferredFormat = GetPreferredCanvasFormat();

  vtxBuffer = CreateBuffer(
      device,
      {.size = cubeVertexArraySize,
       .usage = (BufferUsage)(BufferUsage::VERTEX | BufferUsage::COPY_DST),
       .mappedAtCreation = true});
  
  void* data = BufferGetMappedRange(vtxBuffer, 0, cubeVertexArraySize);

  BufferUnmap(vtxBuffer);

  RenderTarget target;
  target.format = preferredFormat;

  PipelineInfo info = {.layout = 255,
                       .vertex = vertexShader,
                       .fragment = fragmentShader,
                       .targets = &target,
                       .targetCount = 1,
                       .primitiveTopology =
                           GPUPrimitiveTopology::TRIANGLE_LIST};

  pipeline = CreateRenderPipeline(device, info);

  queue = DeviceGetQueue(device);

  request_animation_frame((void *)render_loop);

  //DestroyBuffer(vtxBuffer);
}
