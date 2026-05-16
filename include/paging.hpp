#pragma once
#include <cstdint>
#include <utility>

namespace dart::paging {
  enum class Mode : uint64_t { kStandard = 4 * 1024, kHuge = 2 * 1024 * 1024 };

  constexpr std::pair<const uint64_t, const Mode> AlignSize(uint64_t requested_size) {
    const Mode deducted_mode {
      requested_size >= std::to_underlying(Mode::kHuge) ? Mode::kHuge : Mode::kStandard };
    const uint64_t mode_value = std::to_underlying(deducted_mode);
    const uint64_t aligned_size{ (requested_size + mode_value - 1) & ~(mode_value - 1) };

    return std::pair(
      aligned_size,
      deducted_mode
    );
  }
}
