#pragma once

#include <cstdint>
#include <type_traits>

namespace VtfParser {
  template <typename T>
  concept UnsignedIntegral = std::is_integral_v<T> && std::is_unsigned_v<T>;

  template <typename T>
  concept FloatingPoint = std::is_floating_point_v<T>;

  template <typename T>
  concept ColourChannel = UnsignedIntegral<T> || FloatingPoint<T>;

  template <uint8_t, ColourChannel Channel> struct Colour;

  template <ColourChannel Channel> struct Colour<1, Channel> {
    Channel r = 0;
  };

  template <ColourChannel Channel> struct Colour<2, Channel> {
    Channel r = 0;
    Channel g = 0;
  };

  template <ColourChannel Channel> struct Colour<3, Channel> {
    Channel r = 0;
    Channel g = 0;
    Channel b = 0;
  };

  template <ColourChannel Channel> struct Colour<4, Channel> {
    Channel r = 0;
    Channel g = 0;
    Channel b = 0;
    Channel a = 0;
  };
}
