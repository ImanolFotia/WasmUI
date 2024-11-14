#include "string.hpp"
#include "wgpu.hpp"

struct TextureSize {
  int x, y, z;
};

struct WGPUDevice;
struct WGPUQueue;
struct WGPUTextureView;
struct WGPUTexture;

extern "C" void print(JsString);
extern "C" void request_animation_frame(void *);
extern "C" float trunc(float);

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

float fmodf(float x, float y) { return x - trunc(x / y) * y; }

export auto sum(int a, int b) -> int { return a + b; }

extern "C" void render_callback(float dt) { print("hello!"); }
extern "C" size_t wgpuCreateShaderModule(size_t device, JsString);
extern "C" size_t wgpuGetPreferredCanvasFormat();
extern "C" size_t wgpuCreateRenderPipeline(size_t, PipelineInfo);

export auto hello() -> JsCString {
  return {(size_t) "Hello, world from C++!", 22};
}

struct vec3 {
  float x;
  float y;
  float z;
};

struct marshall_struct {
  int a = 8;
  int b = 5;
  vec3 v;
};

extern "C" void marshall_string(const char *str) { print(str); }

extern "C" auto marshall(void *stru) -> void {
  marshall_struct *ms = (marshall_struct *)stru;
  {
    int a = reinterpret_cast<float &>(ms->a);
    char str[3];
    str[0] = '0' + ((size_t)(a / 10));
    str[1] = '0' + ((size_t)(a % 10));
    str[2] = '\0';
    print("a: ");
    print(str);
  }
  {
    float b = reinterpret_cast<float &>(ms->b);
    char str[6];
    str[0] = '0' + ((size_t)(b / 10));
    str[1] = '0' + ((size_t)(fmodf(b, 10)));

    str[2] = '.';

    str[3] = '0' + ((size_t)(fmodf(b * 10, 10)));
    str[4] = '0' + ((size_t)(fmodf(b * 100, 10)));
    str[5] = '\0';
    print("a: ");
    print(str);
  }

  {
    vec3 v = ms->v;
    char str[6];
    str[0] = '0' + ((size_t)(v.x / 10));
    str[1] = '0' + ((size_t)(fmodf(v.x, 10)));

    str[2] = '.';

    str[3] = '0' + ((size_t)(fmodf(v.x * 10, 10)));
    str[4] = '0' + ((size_t)(fmodf(v.x * 100, 10)));
    str[5] = '\0';
    print("v: ");
    print(str);
  }
}

export auto wasm_main() -> void {
  print("Initiating WebGPU Device");

  size_t device;

  if (device = wgpu::wgpuGetDevice(), device == -1)
    print("device is nullptr\n");


  size_t fragmentShader = wgpuCreateShaderModule(device,
                         R"(@fragment 
                  fn main() -> @location(0) vec4f {
                  return vec4(1.0, 0.0, 0.0, 1.0);
                  })");

  size_t vertexShader = wgpuCreateShaderModule(device,
                         R"(@vertex 
                  fn main(
                  @builtin(vertex_index) VertexIndex : u32
                  ) -> @builtin(position) vec4f {
                  var pos = array<vec2f, 3>(
                      vec2(0.0, 0.5),
                      vec2(-0.5, -0.5),
                      vec2(0.5, -0.5)
                  );
                  return vec4f(pos[VertexIndex], 0.0, 1.0);
                  })");

  size_t preferredFormat = wgpuGetPreferredCanvasFormat();

  RenderTarget target;
  target.format = preferredFormat;

  PipelineInfo info;
  info.layout = 255;
  info.vertex = vertexShader;
  info.fragment = fragmentShader;
  info.targets = &target;
  info.targetCount = 1;
  info.primitiveTopology = GPUPrimitiveTopology::TRIANGLE_LIST;


  wgpuCreateRenderPipeline(device, info);

  /*
      char str[3];
      str[0] = '0' + ((size_t)(device / 10));
      str[1] = '0' + ((size_t)(fmodf(device, 10)));
      str[2] = '\0';
      print(str);

    request_animation_frame((void *)render_callback);*/
}
