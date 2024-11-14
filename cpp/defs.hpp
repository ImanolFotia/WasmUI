#pragma once

#define export extern "C"

namespace {
using size_t = unsigned long;

struct vec3 {
  union {
    struct {
      float x, y, z;
    };
    struct {
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

} // namespace