#include "offset-data-view.hpp"

namespace VtfParser {
  using namespace Errors;

  OffsetDataView::OffsetDataView(const std::weak_ptr<std::vector<std::byte>>& data) : data(data), offset(0) {}

  OffsetDataView::OffsetDataView(const OffsetDataView& from, const size_t newOffset) :
    data(from.data), offset(newOffset) {}

  OffsetDataView OffsetDataView::withRelativeOffset(const size_t newRelativeOffset) const {
    return OffsetDataView(*this, offset + newRelativeOffset);
  }

  std::shared_ptr<std::vector<std::byte>> OffsetDataView::getLockedData() const {
    if (data.expired()) {
      throw std::runtime_error("Attempted to lock an expired weak_ptr to underlying data");
    }

    return data.lock();
  }
}
