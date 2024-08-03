#pragma once

#include "../colours.hpp"
#include <array>
#include <cstdint>
#include <memory>
#include <vector>

/// <summary>
/// DXT compression format functions and structs taken and modified from VTFLib
/// </summary>
namespace VtfParser::Dxt {
  std::vector<Colour<4, uint8_t>> decompressDxt1(
    const std::weak_ptr<std::vector<std::byte>>& src, uint32_t width, uint32_t height
  );

  void decompressDxt3(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height);

  void decompressDxt5(uint8_t* src, uint8_t* dst, uint32_t width, uint32_t height);
}
