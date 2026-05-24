#include <cstring>
#include <iostream>

#include "file.hpp"

int main() {
  try {
    const uint64_t size {dart::file_utils::Megabytes(512)};
    const uint64_t address_pool {size / 8};

    const dart::FileConfig config {
      .path = "benchmark.db",
      .sync_on = 0
    };

    dart::File db = dart::File::Create<size>(config);

    uint8_t* address {db.memory_map()};

    for (uint64_t i{0}; i < address_pool; ++i) {
      const uint64_t offset {i * 8};
      address[offset] = i;
    }
  } catch (const std::bad_expected_access<dart::error_t>& e) {
    std::cerr << std::strerror(e.error());
  }
}