#include <string.hpp>
#include <webgpu.hpp>

struct TextureSize {
  int x, y, z;
};

extern "C" void print(JsString);
extern "C" void request_animation_frame(void *);
extern "C" float trunc(float);
extern "C" void render_callback(float dt) { print("hello!"); }

float fmodf(float x, float y) { return x - trunc(x / y) * y; }

const char *vertexCode =
    R"(
    struct VertexOut {
        @builtin(position) position: vec4f,
        @location(0) color: vec4f,
      }; 
    @vertex fn main(
      @builtin(vertex_index) VertexIndex : u32
      ) -> VertexOut {
      var pos = array<vec2f, 3>(
          vec2(0.0, 0.5),
          vec2(-0.5, -0.5),
          vec2(0.5, -0.5)
      );
      var color = array<vec4f, 3>(
          vec4(1.0, 0.0, 0.0, 1.0),
          vec4(0.0, 1.0, 0.0, 1.0),
          vec4(0.0, 0.0, 1.0, 1.0)
      );
      var vsOut: VertexOut;
      vsOut.position = vec4f(pos[VertexIndex], 0.0, 1.0);
      vsOut.color = color[VertexIndex];
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

size_t device, pipeline, queue;

using namespace wgpu;

void render_loop(float dt) {
  auto encoder = CreateCommandEncoder(device);

  auto textureView = SwapChainGetCurrentTextureView();

  auto pass = CommandEncoderBeginRenderPass(encoder,
                                               {.textureView = textureView,
                                                .clearValue = {0, 0, 0, 1},
                                                .loadOp = LoadOp::CLEAR,
                                                .storeOp = StoreOp::STORE});
  CommandEncoderSetPipeline(pass, pipeline);
  CommandEncoderDraw(pass, 3);
  CommandEncoderEnd(pass);

  QueueSubmit(queue, 1, CommandEncoderFinish(encoder, nullptr));
  CommandEncoderRelease(encoder);
  TextureViewRelease(textureView);
  RenderPassEncoderRelease(pass);
};

export auto wasm_main() -> void {

  if (device = GetDevice(); device == 0) {
    print("Error opening device.");
    return;
  }

  auto fragmentShader = CreateShaderModule(device, fragmentCode);
  auto vertexShader = CreateShaderModule(device, vertexCode);
  auto preferredFormat = GetPreferredCanvasFormat();

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
}
