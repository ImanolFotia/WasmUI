#pragma once

#include <stdint.h>

using ssize_t = long;
using size_t = __SIZE_TYPE__;
using ptrdiff_t = __PTRDIFF_TYPE__;

extern "C" uint32_t getWindowWidth();
extern "C" uint32_t getWindowHeight();

enum StorageTexureAccess {
  WRITE_ONLY,
  READ_ONLY,
  READ_WRITE,
};

enum TextureViewDimension { d1D, d2D, d2D_ARRAY, dCUBE, dCUBE_ARRAY, d3D };

enum BufferBindingType { UNIFORM, STORAGE, READ_ONLY_STORAGE };

enum AddressMode {
  CLAMPTOEDGE = 0,
  REPEAT = 1,
  MIRRORREPEAT = 2,
  CLAMPTOBORDER = 3,
};
enum FilterMode {
  NEAREST = 0,
  LINEAR = 1,
};
enum SamplerBorderColor {
  NONE,
  TRANSPARENTBLACK,
  OPAQUEBLACK,
  OPAQUEWHITE,
  ZERO,
};

enum TextureSampleType {
  FLOAT,
  UNFILTERABLE_FLOAT,
  DEPTH,
  SINT,
  UINT
};

enum SamplerBindingType { FILTERING, NON_FILTERING, COMPARISON };

enum BindGroupLayoutEntryType { BUFFER, SAMPLER, TEXTURE, STORAGE_TEXTURE };

enum ShaderStage { COMPUTE = 4, FRAGMENT = 2, VERTEX = 1 };

enum LoadOp { CLEAR = 0, LOAD };

enum StoreOp { STORE = 0, DISCARD };

enum GPUPrimitiveTopology {
  POINT_LIST,
  LINE_LIST,
  LINE_STRIP,
  TRIANGLE_LIST,
  TRIANGLE_STRIP,
};

enum class TextureDimension { d1D, d2D, d3D };

enum CompareFunction {
  Never = 0,
  Less,
  Equal,
  LessEqual,
  Greater,
  NotEqual,
  GreaterEqual,
  Always,
};

enum TextureFormat {
  R8Unorm = 1,
  R8Snorm,
  R8Uint,
  R8Sint,
  R16Uint,
  R16Sint,
  R16Unorm,
  R16Snorm,
  R16Float,
  Rg8Unorm,
  Rg8Snorm,
  Rg8Uint,
  Rg8Sint,
  R32Uint,
  R32Sint,
  R32Float,
  Rg16Uint,
  Rg16Sint,
  Rg16Unorm,
  Rg16Snorm,
  Rg16Float,
  Rgba8Unorm,
  Rgba8UnormSrgb,
  Rgba8Snorm,
  Rgba8Uint,
  Rgba8Sint,
  Bgra8Unorm,
  Bgra8UnormSrgb,
  Rgb9e5Ufloat,
  Rgb10a2Uint,
  Rgb10a2Unorm,
  Rg11b10Ufloat,
  Rg32Uint,
  Rg32Sint,
  Rg32Float,
  Rgba16Uint,
  Rgba16Sint,
  Rgba16Unorm,
  Rgba16Snorm,
  Rgba16Float,
  Rgba32Uint,
  Rgba32Sint,
  Rgba32Float,
  Stencil8,
  Depth16Unorm,
  Depth24Plus,
  Depth24PlusStencil8,
  Depth32Float,
  Depth32FloatStencil8,
  NV12,
  Bc1RgbaUnorm,
  Bc1RgbaUnormSrgb,
  Bc2RgbaUnorm,
  Bc2RgbaUnormSrgb,
  Bc3RgbaUnorm,
  Bc3RgbaUnormSrgb,
  Bc4RUnorm,
  Bc4RSnorm,
  Bc5RgUnorm,
  Bc5RgSnorm,
  Bc6hRgbUfloat,
  Bc6hRgbFloat,
  Bc7RgbaUnorm,
  Bc7RgbaUnormSrgb,
  Etc2Rgb8Unorm,
  Etc2Rgb8UnormSrgb,
  Etc2Rgb8A1Unorm,
  Etc2Rgb8A1UnormSrgb,
  Etc2Rgba8Unorm,
  Etc2Rgba8UnormSrgb,
  EacR11Unorm,
  EacR11Snorm,
  EacRg11Unorm,
  EacRg11Snorm,
  AstcBlock,
  AstcChannel,

};

enum class BufferUsage : size_t {
  MAP_READ = 1,
  MAP_WRITE = 2,
  COPY_SRC = 4,
  COPY_DST = 8,
  INDEX = 16,
  VERTEX = 32,
  UNIFORM = 64,
  STORAGE = 128,
  INDIRECT = 256,
  QUERY_RESOLVE = 512
};

static size_t operator|(BufferUsage a, BufferUsage b) {
  return (size_t)a | (size_t)b;
}

static size_t operator&(BufferUsage a, BufferUsage b) {
  return (size_t)a & (size_t)b;
}

enum class TextureAspect { ALL, STENCIL_ONLY, DEPTH_ONLY };

enum class TextureUsage : size_t {
  COPY_SRC = 0x01,
  COPY_DST = 0x02,
  TEXTURE_BINDING = 0x04,
  STORAGE_BINDING = 0x08,
  RENDER_ATTACHMENT = 0x10
};

static TextureUsage operator|(TextureUsage a, TextureUsage b) {
  return (TextureUsage)((size_t)a | (size_t)b);
}

static TextureUsage operator&(TextureUsage a, TextureUsage b) {
  return (TextureUsage)((size_t)a & (size_t)b);
}

enum VertexFormat : size_t {
  Uint8x2 = 0,
  Uint8x4 = 1,
  Sint8x2 = 2,
  Sint8x4 = 3,
  Unorm8x2 = 4,
  Unorm8x4 = 5,
  Snorm8x2 = 6,
  Snorm8x4 = 7,
  Uint16x2 = 8,
  Uint16x4 = 9,
  Sint16x2 = 10,
  Sint16x4 = 11,
  Unorm16x2 = 12,
  Unorm16x4 = 13,
  Snorm16x2 = 14,
  Snorm16x4 = 15,
  Float16x2 = 16,
  Float16x4 = 17,
  Float32 = 18,
  Float32x2 = 19,
  Float32x3 = 20,
  Float32x4 = 21,
  Uint32 = 22,
  Uint32x2 = 23,
  Uint32x3 = 24,
  Uint32x4 = 25,
  Sint32 = 26,
  Sint32x2 = 27,
  Sint32x3 = 28,
  Sint32x4 = 29,
  Float64 = 30,
  Float64x2 = 31,
  Float64x3 = 32,
  Float64x4 = 33,
  Unorm10_10_10_2 = 34,
};
