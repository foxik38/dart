#pragma once
#include <cstdint>

namespace dart {
  struct Config {
    char name[32];
    uint64_t size;
    uint32_t sync_on{0};
    bool read_only{false};
  };
}