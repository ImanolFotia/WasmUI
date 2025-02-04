// #include "cube.hpp"
#include <arena.hpp>
#include <string.hpp>
#include <webgpu.hpp>

#include <math/matrix.hpp>
#include <promise.hpp>
#include <std/stdio.hpp>

#include "primitives/cube.hpp"
#include "primitives/cylinder.hpp"
#include "primitives/quad.hpp"

extern "C" void request_animation_frame(void *);
extern "C" FILE fopen(JsString);
extern "C" void fread(FILE, char *, size_t);

const char *vertexCode =
    /* wgsl */
    R"(
struct Uniforms {
  viewProj : mat4x4f,
  time: f32,
  civ_uv: vec2f
};
@binding(0) @group(0) var<uniform> uniforms : Uniforms;
struct VertexOut {
    @builtin(position) Position: vec4f,
    @location(0) uv: vec2f,
    @location(1) normal: vec3f,
    @location(2) time: f32,
    @location(3) fragPos: vec3f
  }; 
struct VertexIn {
  @location(0) position: vec3f,
  @location(1) uv: vec2f,
  @location(2) normal: vec3f
};
@vertex fn main( vtx: VertexIn ) -> VertexOut {
  var vsOut: VertexOut;
  vsOut.Position = uniforms.viewProj * vec4f(vtx.position, 1.0);
  vsOut.fragPos = vsOut.Position.xyz;
  var uv: vec2f = vec2f(1.0-vtx.uv.x, 1.0-vtx.uv.y);
  vsOut.uv = (uv * (1.0/9.0)) + uniforms.civ_uv;
  vsOut.normal = vtx.normal;
  vsOut.time = uniforms.time;
  return vsOut;
})";

const char *fragmentCode =
    R"(
@group(0) @binding(1) var mySampler: sampler;
@group(0) @binding(2) var myTexture: texture_2d<f32>;
 struct FragmentIn {
    @location(0) uv: vec2f,
    @location(1) normal: vec3f,
    @location(2) time: f32,
    @location(3) fragPos: vec3f
  }; 
  @fragment fn main(fsInput: FragmentIn) -> @location(0) vec4f {
  var albedo: vec4f = textureSample(myTexture, mySampler, fsInput.uv);
  if(albedo.a < 0.1){ 
    discard;
  }
  return albedo;
})";

using namespace wgpu;

Device device{};
Pipeline pipeline{};
Queue queue{};
Buffer vtxBuffer{};
Buffer idxBuffer{};
Buffer uniformBuffer{};
BindGroup bindGroup[3];
Texture depthBuffer{};
Texture cubeTexture{};

struct Object {
  Math::mat4 transform;
  Buffer vtxBuffer;
  Buffer idxBuffer;
  Buffer uniformBuffer;
  BindGroup bindGroup;
  uint32_t uniform_offset;
  uint32_t vtx_count;
  uint32_t idx_count;
};

struct UniformBuffer {
  Math::mat4 viewProj;
  float time;
  float padding[1]; // padding <-- VERY IMPORTANT
  Math::vec2 civ_uv;
};

size_t cube_vertex_size = 0;
size_t cube_index_size = 0;
size_t indexCount = 0;

const int num_civs = 45;

const char *civilization_names[num_civs] = {
    "Armenians", "Azteks",     "Bengalis",   "Berbers",
    "Bohemians", "Britons",    "Bulgarians", "Burgundiars",
    "Burmese",   "Byzantines", "Celts", "Chinese", "Cumans",
    "Dravidians", "Ethiopians", "Franks", "Georgians", "Goths",
    "Gurjaras", "Hindustanis", "Huns", "Incas", "Italians", 
    "Japanese", "Khmer", "Koreans", "Lithuanians", "Magyars",
    "Malay", "Malians", "Mayans", "Mongols", "Persians", "Poles",
    "Portuguese", "Romans", "Saracens", "Sicilians", "Slavs",
    "Spanish", "Tatars", "Teutons", "Turks", "Vietnamese", "Vikings"
};

enum class Region : uint8_t {
  AMERICAS = 0,
  AFRICA,
  EUROPE,
  ASIA,
};

enum class CivFlags : uint32_t {
  NONE = 0,
  HAS_ELEPHANTS = 1 << 0,
  HAS_BOMBSHIPS = 1 << 1,
};

struct Civilization {
  uint8_t id = 0;
  const char* name;
  CivFlags flags = CivFlags::NONE;
  Region region;
  Math::vec2 civ_uv;
};

Civilization civilizations[num_civs] = {0};

void InitCivilizations() {

  for(uint8_t i = 0; i < num_civs; i++) {
    civilizations[i] =  {.id = i, .name = civilization_names[i],  .flags = CivFlags::NONE, .region = Region::EUROPE};
  }
  civilizations[0] =  {.id = 0,.name = civilization_names[0],  .flags = CivFlags::NONE, .region = Region::EUROPE};
  civilizations[1] =  {.id = 2,.name = civilization_names[1],  .flags = CivFlags::NONE, .region = Region::AMERICAS};
  civilizations[2] =  {.id = 3,.name = civilization_names[2],  .flags = CivFlags::HAS_ELEPHANTS, .region = Region::ASIA};
  civilizations[3] =  {.id = 4,.name = civilization_names[3],  .flags = CivFlags::HAS_ELEPHANTS, .region = Region::ASIA};
  civilizations[4] =  {.id = 5,.name = civilization_names[4],  .flags = CivFlags::NONE, .region = Region::EUROPE};
  civilizations[5] =  {.id = 6,.name = civilization_names[5],  .flags = CivFlags::HAS_BOMBSHIPS, .region = Region::EUROPE};
  civilizations[6] =  {.id =7,.name = civilization_names[6],  .flags = CivFlags::NONE, .region = Region::EUROPE};
  civilizations[7] =  {.id = 0,.name = civilization_names[7],  .flags = CivFlags::NONE, .region = Region::EUROPE};
  civilizations[8] =  {.id = 0,.name = civilization_names[8],  .flags = CivFlags::NONE, .region = Region::AFRICA};
  civilizations[9] =  {.id = 4,.name = civilization_names[9],  .flags = CivFlags::NONE, .region = Region::EUROPE};
  civilizations[10] = {.id = 0,.name = civilization_names[10], .flags = CivFlags::NONE, .region = Region::EUROPE};
  civilizations[11] = {.id = 5,.name = civilization_names[11], .flags = CivFlags::NONE, .region = Region::ASIA};
  civilizations[12] = {.id = 7,.name = civilization_names[12], .flags = CivFlags::NONE, .region = Region::ASIA};
  civilizations[13] = {.id = 0,.name = civilization_names[13], .flags = CivFlags::NONE, .region = Region::ASIA};
}

struct Card {
  float angle;
  Object renderObject;
  Civilization *civilization = nullptr;
};

uint8_t num_cards = 3;
Card cards[5] = {
  {.angle = 0.0, .civilization = &civilizations[0]},
  {.angle = 0.0, .civilization = &civilizations[1]},
  {.angle = 0.0, .civilization = &civilizations[2]},
  {.angle = 0.0, .civilization = &civilizations[0]},
  {.angle = 0.0, .civilization = &civilizations[0]}
};


const int grid_size = 9;
const float item_width = 1.0f / (float)grid_size;
const float item_height = 1.0f / (float)grid_size;

Math::vec2 getCivCoord(unsigned id) {
  return {((float)(id % grid_size)) * item_width,
          ((float)(id / grid_size)) * item_height};
}

int randomCiv() { return Math::floor(Math::rand() * num_civs); }

void updateCard(int index, float dt) {
  float &angle = cards[index].angle;
  angle += dt * 6.0;

  if (angle >= 2.0) {

    int civ = randomCiv();
    while (civ != cards[index].civilization->id)
      if (cards[index].civilization->id != civ) {
        cards[index].civilization = &civilizations[civ];
        angle = 1;
        civ = randomCiv();
      }
  }
}


void build_pipeline() {

  ColorTargetState target;
  target.format = GetPreferredCanvasFormat();

  VertexBufferLayout bufferLayout;
  bufferLayout.arrayStride = sizeof(Math::Vertex);
  bufferLayout.attributeCount = 3;
  VertexAttribute attributes[3] = {{.format = Float32x3,
                                    .offset = offsetof(Math::Vertex, position),
                                    .shaderLocation = 0},
                                   {.format = Float32x2,
                                    .offset = offsetof(Math::Vertex, texCoord),
                                    .shaderLocation = 1},
                                   {.format = Float32x3,
                                    .offset = offsetof(Math::Vertex, normal),
                                    .shaderLocation = 2}};

  bufferLayout.attributes = attributes;

  BufferBindingLayout buf = {};
  buf.type = BufferBindingType::UNIFORM;

  SamplerBindingLayout samp;
  samp.type = SamplerBindingType::FILTERING;

  TextureBindingLayout tex = {.sampleType = TextureSampleType::FLOAT,
                              .viewDimension = TextureViewDimension::d2D,
                              .multisampled = false};

  BindGroupLayoutEntry bglEntries[3] = {{
                                            .binding = 0,
                                            .visibility = ShaderStage::VERTEX,
                                            .buffer = &buf,
                                        },
                                        {
                                            .binding = 1,
                                            .visibility = ShaderStage::FRAGMENT,
                                            .sampler = &samp,
                                        },
                                        {
                                            .binding = 2,
                                            .visibility = ShaderStage::FRAGMENT,
                                            .texture = &tex,
                                        }};

  BindGroupLayoutDescriptor bglDesc = {};
  bglDesc.entryCount = 3;
  bglDesc.entries = bglEntries;
  BindGroupLayout bindGroupLayout = CreateBindGroupLayout(device, bglDesc);

  PipelineLayoutDescriptor layout_descriptor;

  layout_descriptor.bindGroupLayoutCount = 1;
  layout_descriptor.bindGroupLayouts = &bindGroupLayout;

  SamplerDescriptor samplerDesc = {
      .address_mode_u = REPEAT,
      .address_mode_v = REPEAT,
      .mag_filter = FilterMode::LINEAR,
      .min_filter = FilterMode::LINEAR,
  };

  Sampler sampler = CreateSampler(device, samplerDesc);

  BindGroupEntry bgEntries0[3] = {
      {.binding = 0,
       .resource = {.buffer = uniformBuffer,
                    .offset = 0,
                    .size = sizeof(UniformBuffer)}},
      {.binding = 1, .resource = {.sampler = sampler}},
      {.binding = 2,
       .resource = {.textureView = CreateTextureView(cubeTexture)}}};

  BindGroupDescriptor bindGroupDescriptor;
  bindGroupDescriptor.layout = bindGroupLayout;
  bindGroupDescriptor.entryCount = 3;
  bindGroupDescriptor.entries = bgEntries0;
  bindGroup[0] = CreateBindGroup(device, bindGroupDescriptor);

  BindGroupEntry bgEntries1[3] = {
      {.binding = 0,
       .resource = {.buffer = uniformBuffer,
                    .offset = 256,
                    .size = sizeof(UniformBuffer)}},
      {.binding = 1, .resource = {.sampler = sampler}},
      {.binding = 2,
       .resource = {.textureView = CreateTextureView(cubeTexture)}}};

  bindGroupDescriptor.entries = bgEntries1;
  bindGroup[1] = CreateBindGroup(device, bindGroupDescriptor);

  BindGroupEntry bgEntries2[3] = {
      {.binding = 0,
       .resource = {.buffer = uniformBuffer,
                    .offset = 512,
                    .size = sizeof(UniformBuffer)}},
      {.binding = 1, .resource = {.sampler = sampler}},
      {.binding = 2,
       .resource = {.textureView = CreateTextureView(cubeTexture)}}};

  bindGroupDescriptor.entries = bgEntries2;
  bindGroup[2] = CreateBindGroup(device, bindGroupDescriptor);

  PipelineLayout pipeline_layout =
      CreatePipelineLayout(device, layout_descriptor);

  RenderPipelineDescriptor info = {
      .layout = pipeline_layout,
      .vertex = {.module = CreateShaderModule(device, vertexCode),
                 .entryPoint = "main",
                 .buffersCount = 1,
                 .buffers = &bufferLayout},
      .primitive = {.topology = GPUPrimitiveTopology::TRIANGLE_LIST},
      .depthStencil =
          {
              .format = TextureFormat::Depth24Plus,
              .depthWriteEnabled = true,
              .depthCompare = CompareFunction::Less,
          },
      .fragment = {.module = CreateShaderModule(device, fragmentCode),
                   .entryPoint = "main",
                   .targetCount = 1,
                   .targets = &target},
  };
  pipeline = CreateRenderPipeline(device, info);

  depthBuffer = CreateTexture(
      device, {
                  .size = {(int)getWindowWidth(), (int)getWindowHeight(), 0},
                  .dimension = TextureDimension::d2D,
                  .format = TextureFormat::Depth24Plus,
                  .usage = TextureUsage::RENDER_ATTACHMENT,
              });

  ReleaseBindGroupLayout(bindGroupLayout);
}

float time = 0.0f;

struct RenderState {
  CommandEncoder encoder;
  TextureView textureView;
  TextureView depthView;
};

RenderState state;

RenderPass RenderBegin() {
 state.encoder = CreateCommandEncoder(device);

  state.textureView = SwapChainGetCurrentTextureView();
  state.depthView = CreateTextureView(depthBuffer);

  RenderPassColorAttachment colorAttachment;
  colorAttachment.view = state.textureView;
  colorAttachment.clearValue = {0.0f, 0.0f, 0.0f, 0.0};
  colorAttachment.operations = {LoadOp::CLEAR, StoreOp::STORE};

  RenderPassDepthStencilAttachment depthAttachment;
  depthAttachment.view = state.depthView;
  depthAttachment.clearValue = {1.0f, 1.0f};
  depthAttachment.depthOps = {LoadOp::CLEAR, StoreOp::STORE};

  RenderPass pass = CommandEncoderBeginRenderPass(
      state.encoder,
      (RenderPassDescriptor){.colorAttachmentsCount = 1,
                             .colorAttachments = &colorAttachment,
                             .depthStencilAttachmentsCount = 1,
                             .depthStencilAttachments = &depthAttachment});
  return pass;
}

void RenderEnd(RenderPass pass) {


  RenderPassEncoderEnd(pass);
  RenderPassEncoderRelease(pass);
  CommandBuffer finish = CommandEncoderFinish(state.encoder, nullptr);
  CommandEncoderRelease(state.encoder);
  QueueSubmit(queue, 1, finish);

  CommandBufferRelease(finish);
  TextureViewRelease(state.textureView);
  TextureViewRelease(state.depthView);
}

void Draw(RenderPass pass, const Card &card) {


  UniformBuffer uBuffer = {
      .viewProj = card.renderObject.transform, .time = time, .civ_uv = getCivCoord(card.civilization->id)};
  QueueWriteBuffer(queue, card.renderObject.uniformBuffer, card.renderObject.uniform_offset, (char *)&uBuffer, sizeof(UniformBuffer));

  RenderPassEncoderSetPipeline(pass, pipeline);
  RenderPassEncoderSetVertexBuffer(pass, 0, card.renderObject.vtxBuffer, 0, card.renderObject.vtx_count * sizeof(Math::Vertex));
  RenderPassEncoderSetIndexBuffer(pass, card.renderObject.idxBuffer, 0, 0, card.renderObject.idx_count * sizeof(uint32_t));

  RenderPassEncoderSetBindGroup(pass, 0, card.renderObject.bindGroup, 0, nullptr);
  RenderPassEncoderDrawIndexed(pass, card.renderObject.idx_count);
  
}

extern "C" void render_loop(float dt) {
  time += dt;
 
  using Math::mat4;
  using Math::vec2;
  using Math::vec3;

  uint32_t width = getWindowWidth();
  uint32_t height = getWindowHeight();

  mat4 view = Math::lookAt(vec3(0.0f, 0.0f, 6.0f), vec3(0.0f, 0.0, 0.0f),
                           vec3(0.0f, 1.0f, 0.0f));

  mat4 proj = Math::perspective(Math::radians(30.0f), width / (height + 0.001),
                                0.1f, 100.0f);

  auto pass = RenderBegin();
  float x = -1.5;
  for(int i = 0; i < num_cards; i++) {
    Card *card = &cards[i];
    updateCard(i, dt);
    mat4 r = Math::rotate(Math::pi * card->angle, vec3(1.0, 0.0, 0.0));
    mat4 t = Math::translate(vec3(x, -1.0, 0.0));
    mat4 s = Math::scale(vec3(0.75f));
    mat4 transform = proj * view * t * r * s;
    card->renderObject.transform = transform;
    card->renderObject.bindGroup = bindGroup[i];
    x += 1.5;
    Draw(pass, cards[i]);
  }
  RenderEnd(pass);
};

int main(int argc, char **argv) {

  if (device = GetDevice(); device == 0) {
    puts("Error opening device.");
    return 1;
  }

  queue = DeviceGetQueue(device);

  ShaderModule fragmentShader = CreateShaderModule(device, fragmentCode);
  ShaderModule vertexShader = CreateShaderModule(device, vertexCode);

  engine::Quad cube;
  indexCount = cube.data().num_indices;
  cube_vertex_size = cube.data().num_vertices * sizeof(Math::Vertex);
  cube_index_size = indexCount * sizeof(uint32_t);

  vtxBuffer = CreateBuffer(device, {.size = cube_vertex_size,
                                    .usage = (BufferUsage)(BufferUsage::VERTEX),
                                    .mappedAtCreation = true});

  idxBuffer = CreateBuffer(device, {.size = cube_index_size,
                                    .usage = (BufferUsage)(BufferUsage::INDEX),
                                    .mappedAtCreation = true});

  {
    void *data = BufferGetMappedRange(vtxBuffer, 0, cube_vertex_size);
    std::memcpy(data, (void *)cube.data().Vertices, cube_vertex_size);
    BufferUnmap(vtxBuffer);
  }
  {
    void *data = BufferGetMappedRange(idxBuffer, 0, cube_index_size);
    std::memcpy(data, (void *)cube.data().Indices, cube_index_size);
    BufferUnmap(idxBuffer);
  }

  uniformBuffer = CreateBuffer(
      device,
      {.size = 256 + 256 + 80 * 3,
       .usage = (BufferUsage)(BufferUsage::UNIFORM | BufferUsage::COPY_DST),
       .mappedAtCreation = false});

       //offsets 0, 256, 512

/*

  Math::mat4 transform;
  Buffer vtxBuffer;
  Buffer idxBuffer;
  Buffer uniformBuffer;
  BindGroup bindGroup;
  uint32_t uniform_offset;
  uint32_t vtx_count;
  uint32_t idx_count;
*/

InitCivilizations();
  for(int i = 0; i < num_cards; i++) {
    cards[i].renderObject.vtxBuffer = vtxBuffer;
    cards[i].renderObject.idxBuffer = idxBuffer;
    cards[i].renderObject.uniformBuffer = uniformBuffer;
    cards[i].renderObject.uniformBuffer = uniformBuffer;
    cards[i].renderObject.vtx_count = cube.data().num_vertices;
    cards[i].renderObject.idx_count = indexCount;
    cards[i].renderObject.uniform_offset = i * 256;
  }

  cubeTexture =
      CreateTexture(device, {
                                .size = {4096, 4096, 0},
                                .dimension = TextureDimension::d2D,
                                .format = TextureFormat::Rgba8Unorm,
                                .usage = TextureUsage::RENDER_ATTACHMENT |
                                         TextureUsage::COPY_DST |
                                         TextureUsage::TEXTURE_BINDING,
                            });

  fetch("../../media/aoe2/civs.png").then([](Response response) {
    response.blob().then([](Blob blob) {
      createImageBitmap(blob.handle).then([](Image image) {
        QueueCopyExternalImageToTexture(queue, (void *)image.handle,
                                        cubeTexture, {4096, 4096, 0});
      });
    });
  });

  build_pipeline();

#ifdef __wasm__
  request_animation_frame((void *)render_loop);
#endif

#ifndef __wasm__
  DestroyBuffer(vtxBuffer);
#endif
}
