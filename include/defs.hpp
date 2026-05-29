#pragma once
#include <cstdint>

namespace dart {
  struct TableData {
    char name[32];
    uint64_t size;
  };

  struct Flags {
    uint32_t sync_on{0};
    bool read_only{false};
  };
}