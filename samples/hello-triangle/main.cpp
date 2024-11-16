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

float min(float a, float b) {
  if(a > b) return a;
  return b;
}

float max(float a, float b) {
  if(a > b) return b;
  return a;
}

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

using namespace wgpu;

Device device{};
Pipeline pipeline{};
Queue queue{};

void render_loop(size_t dt) {
  char str[5];

  str[0] = '0' + fmodf((dt / 1000), 10);
  str[1] = '0' + fmodf((dt / 100), 10);
  str[2] = '0' +  fmodf(dt / 10, 10);
  str[3] = '0' + fmodf(dt, 10);
  str[4] = 0;

  print(str);
  //float t = (float)dt;
  //(void)t;

  CommandEncoder encoder = CreateCommandEncoder(device);

  TextureView textureView = SwapChainGetCurrentTextureView();

  RenderPass pass = CommandEncoderBeginRenderPass(encoder,
                                               {.textureView = textureView,
                                                .clearValue = {0, 0, 0, 1},
                                                .loadOp = LoadOp::CLEAR,
                                                .storeOp = StoreOp::STORE});
  CommandEncoderSetPipeline(pass, pipeline);
  CommandEncoderDraw(pass, 3);
  CommandEncoderEnd(pass);
  CommandBuffer finish = CommandEncoderFinish(encoder, nullptr);
  QueueSubmit(queue, 1, finish);

  RenderCommandBufferRelease(finish);
  CommandEncoderRelease(encoder);
  TextureViewRelease(textureView);
  RenderPassEncoderRelease(pass);
};

extern "C" auto wasm_main() -> void {

  if (device = GetDevice(); device == 0) {
    print("Error opening device.");
    return;
  }

  ShaderModule fragmentShader = CreateShaderModule(device, fragmentCode);
  ShaderModule vertexShader = CreateShaderModule(device, vertexCode);
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

  request_animation_frame((void*)render_loop);
}
