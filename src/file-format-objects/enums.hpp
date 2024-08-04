#pragma once

#include <cstdint>
namespace VtfParser {
  enum class ImageFormat : int32_t // NOLINT(*-enum-size)
  {
    NONE = -1,
    RGBA8888 = 0,
    ABGR8888,
    RGB888,
    BGR888,
    RGB565,
    I8,
    IA88,
    P8,
    A8,
    RGB888_BLUESCREEN,
    BGR888_BLUESCREEN,
    ARGB8888,
    BGRA8888,
    DXT1,
    DXT3,
    DXT5,
    BGRX8888,
    BGR565,
    BGRX5551,
    BGRA4444,
    DXT1_ONEBITALPHA,
    BGRA5551,
    UV88,
    UVWQ8888,
    RGBA16161616F,
    RGBA16161616,
    UVLX8888
  };

  enum class TextureFlags : uint32_t {
    NONE = 0,
    POINTSAMPLE = 0x00000001,
    TRILINEAR = 0x00000002,
    CLAMPS = 0x00000004,
    CLAMPT = 0x00000008,
    ANISOTROPIC = 0x00000010,
    HINT_DXT5 = 0x00000020,
    PWL_CORRECTED = 0x00000040,
    NORMAL = 0x00000080,
    NOMIP = 0x00000100,
    NOLOD = 0x00000200,
    ALL_MIPS = 0x00000400,
    PROCEDURAL = 0x00000800,

    ONEBITALPHA = 0x00001000,
    EIGHTBITALPHA = 0x00002000,

    ENVMAP = 0x00004000,
    RENDERTARGET = 0x00008000,
    DEPTHRENDERTARGET = 0x00010000,
    NODEBUGOVERRIDE = 0x00020000,
    SINGLECOPY = 0x00040000,
    PRE_SRGB = 0x00080000,

    UNUSED_00100000 = 0x00100000,
    UNUSED_00200000 = 0x00200000,
    UNUSED_00400000 = 0x00400000,

    NODEPTHBUFFER = 0x00800000,

    UNUSED_01000000 = 0x01000000,

    CLAMPU = 0x02000000,
    VERTEXTEXTURE = 0x04000000,
    SSBUMP = 0x08000000,

    UNUSED_10000000 = 0x10000000,

    BORDER = 0x20000000,

    UNUSED_40000000 = 0x40000000,
    UNUSED_80000000 = 0x80000000
  };
  inline TextureFlags operator&(const TextureFlags& a, const TextureFlags& b) {
    return static_cast<TextureFlags>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
  }
  inline TextureFlags operator|(const TextureFlags& a, const TextureFlags& b) {
    return static_cast<TextureFlags>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
  }
}
