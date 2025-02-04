#include <stdio.hpp>
#include <string.hpp>
#include <webgpu.hpp>
#include <arena.hpp>

struct TextureSize {
  int x, y, z;
};

extern "C" void request_animation_frame(void *);
extern "C" float trunc(float);

extern "C" FILE fopen(JsString);
extern "C" void fread(FILE, char *, size_t);

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

FILE file;

extern "C" void render_loop(size_t dt) {
  /*char str[5];

  str[0] = '0' + fmodf((dt / 1000), 10);
  str[1] = '0' + fmodf((dt / 100), 10);
  str[2] = '0' +  fmodf(dt / 10, 10);
  str[3] = '0' + fmodf(dt, 10);
  str[4] = 0;

  print(str);*/
  // float t = (float)dt;
  //(void)t;

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
  RenderPassEncoderDraw(pass, 3);
  RenderPassEncoderEnd(pass);
  CommandBuffer finish = CommandEncoderFinish(encoder, nullptr);
  QueueSubmit(queue, 1, finish);

  CommandBufferRelease(finish);
  CommandEncoderRelease(encoder);
  TextureViewRelease(textureView);
  RenderPassEncoderRelease(pass);

};

int main(int argc, char** argv) {

  if (device = GetDevice(); device == 0) {
    puts("Error opening device.");
    return 1;
  }

  ShaderModule fragmentShader = CreateShaderModule(device, fragmentCode);
  ShaderModule vertexShader = CreateShaderModule(device, vertexCode);
  auto preferredFormat = GetPreferredCanvasFormat();

  ColorTargetState target;
  target.format = preferredFormat;

  RenderPipelineDescriptor info = {
      .layout = {},
      .vertex = {.module = vertexShader, .entryPoint = "main"},
      .primitive = {.topology = GPUPrimitiveTopology::TRIANGLE_LIST},
      .fragment = {.module = fragmentShader,
                   .entryPoint = "main",
                   .targetCount = 1,
                   .targets = &target}};

  pipeline = CreateRenderPipeline(device, info);

  queue = DeviceGetQueue(device);

  request_animation_frame((void *)render_loop);
}
