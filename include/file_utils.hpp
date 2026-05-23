#pragma once
#include <cstdint>
#include <utility>

namespace dart::file_utils {
  enum class Unit : uint64_t {
    kBytes = 1,
    kKilobytes = 1024,
    kMegabytes = 1024 * 1024
  };

  enum class Page : uint64_t {
    kStandard = 4 * std::to_underlying(Unit::kKilobytes),
    kHuge = 2 * std::to_underlying(Unit::kMegabytes)
  };

  constexpr uint64_t Bytes(uint64_t bytes) {
    return bytes * std::to_underlying(Unit::kBytes);
  }

  constexpr uint64_t Kilobytes(uint64_t kilobytes) {
    return kilobytes * std::to_underlying(Unit::kKilobytes);
  }

  constexpr uint64_t Megabytes(uint64_t megabytes) {
    return megabytes * std::to_underlying(Unit::kMegabytes);
  }

  constexpr std::pair<uint64_t, Page> Align(uint64_t requested_size) {
    const Page deducted_mode {
      requested_size >= std::to_underlying(Page::kHuge) ? Page::kHuge : Page::kStandard };
    const uint64_t mode_value = std::to_underlying(deducted_mode);
    const uint64_t aligned_size{ (requested_size + mode_value - 1) & ~(mode_value - 1) };

    return std::pair{
      aligned_size,
      deducted_mode
    };
  }
}
