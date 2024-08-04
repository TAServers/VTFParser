#pragma once

#include "check-bounds.hpp"
#include <memory>
#include <vector>

namespace VtfParser {
  class OffsetDataView {
  public:
    explicit OffsetDataView(const std::weak_ptr<std::vector<std::byte>>& data);

    explicit OffsetDataView(const OffsetDataView& from, const size_t newOffset);

    [[nodiscard]] OffsetDataView withRelativeOffset(const size_t newRelativeOffset) const;

    template <typename T> [[nodiscard]] T parseStruct(const size_t relativeOffset, const char* errorMessage) const {
      const auto lockedData = getLockedData();
      const auto absoluteOffset = offset + relativeOffset;
      checkBounds(absoluteOffset, sizeof(T), lockedData->size(), errorMessage);

      return *reinterpret_cast<const T*>(&lockedData->at(absoluteOffset));
    }

  private:
    std::weak_ptr<std::vector<std::byte>> data;
    size_t offset;

    [[nodiscard]] std::shared_ptr<std::vector<std::byte>> getLockedData() const;
  };
}
