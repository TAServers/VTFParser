#pragma once

#include "../errors.hpp"
#include <cstddef>

namespace VtfParser {
  inline void checkBounds(size_t offset, size_t count, size_t rangeSize, const char* errorMessage) {
    if (offset >= rangeSize || offset + count > rangeSize) {
      throw Errors::OutOfBoundsAccess(errorMessage);
    }
  }
}
