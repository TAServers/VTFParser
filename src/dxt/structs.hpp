#pragma once

#include <cstdint>

namespace VtfParser::Dxt {
  // RGB Pixel type
  struct Colour888 {
    uint8_t r; // change the order of names to change the
    uint8_t g; // order of the output ARGB or BGRA, etc...
    uint8_t b; // Last one is MSB, 1st is LSB.
  };

  // BGR565 Pixel type
  struct Colour565 {
    uint32_t b : 5; // order of names changes
    uint32_t g : 6; // byte order of output to 32 bit
    uint32_t r : 5;
  };

  // RGBA Pixel type
  struct Colour8888 {
    uint8_t r; // change the order of names to change the
    uint8_t g; // order of the output ARGB or BGRA, etc...
    uint8_t b; // Last one is MSB, 1st is LSB.
    uint8_t a;

    Colour8888() : r(0), g(0), b(0), a(255) {}
    Colour8888(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) : r(r), g(g), b(b), a(a) {}

    explicit Colour8888(const Colour565& rgb, const uint8_t alpha) :
      r(rgb.r << 3u), g(rgb.g << 2u), b(rgb.b << 3u), a(alpha) {}
  };

  // DXTn Colour block type
  struct DXTColBlock {
    int16_t col0;
    int16_t col1;
    std::array<int8_t, 4> row; // no bit fields - use bytes
  };

  // DXTn Alpha block types
  struct DXTAlphaBlockExplicit {
    std::array<uint16_t, 4> row;
  };

  struct DXTAlphaBlock3BitLinear {
    int8_t alpha0;
    int8_t alpha1;
    std::array<int8_t, 6> unused;
  };
}
