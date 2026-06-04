#pragma once
#include <cstdint>
#include <utility>

namespace dart::file_utils
{
  enum class unit : uint64_t {
    bytes = 1,
    kilobytes = 1024,
    megabytes = 1024 * 1024
  };

  enum class page : uint64_t {
    standard = 4 * std::to_underlying(unit::kilobytes),
    huge = 2 * std::to_underlying(unit::megabytes)
  };

  constexpr uint64_t bytes(uint64_t bytes)
  {
    return bytes * std::to_underlying(unit::bytes);
  }

  constexpr uint64_t kilobytes(uint64_t kilobytes)
  {
    return kilobytes * std::to_underlying(unit::kilobytes);
  }

  constexpr uint64_t megabytes(uint64_t megabytes)
  {
    return megabytes * std::to_underlying(unit::megabytes);
  }

  //needs recode
  constexpr std::pair<uint64_t, page> align(uint64_t requested_size)
  {
    const page deducted_mode{requested_size >= std::to_underlying(page::huge)
                                 ? page::huge
                                 : page::standard};
    const uint64_t mode_value = std::to_underlying(deducted_mode);
    const uint64_t aligned_size{(requested_size + mode_value - 1) &
                                ~(mode_value - 1)};

    return std::pair{aligned_size, deducted_mode};
  }
} // namespace dart::file_utils
