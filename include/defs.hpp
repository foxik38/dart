#pragma once
#include <cstdint>

namespace dart
{
  struct table_data {
    char name[32];
    uint64_t size;
  };

  struct table_flags {
    uint32_t sync_on{0};
    bool read_only{false};
  };
} // namespace dart