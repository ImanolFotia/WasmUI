#pragma once

using ssize_t = long;
using size_t = __SIZE_TYPE__;
using ptrdiff_t = __PTRDIFF_TYPE__; 


struct vec3 {
  union impl_vec3 {
    struct vec {
      float x, y, z;
    };
    struct col {
      float r, g, b;
    };
    float val[3];
  };
};

enum LoadOp { CLEAR, LOAD };

enum StoreOp { STORE, DISCARD };

enum GPUPrimitiveTopology {
  POINT_LIST,
  LINE_LIST,
  LINE_STRIP,
  TRIANGLE_LIST,
  TRIANGLE_STRIP,
};

enum TextureFormat {
  R8Unorm = 0,
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

enum BufferUsage : size_t {
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
