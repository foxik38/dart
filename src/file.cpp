#include "file.hpp"

#include <sys/mman.h>
#include <unistd.h>

namespace dart {

  std::expected<void, error_t> File::Open() noexcept {
    return {};
  }

  std::expected<void, error_t> File::MapMemory() noexcept {
    return {};
  }

  void File::CloseAndUnmapMemory() noexcept {
    if (file_descriptor_ != -1) {
      close(file_descriptor_);
      file_descriptor_ = -1;
    }

    if (memory_map_) {
      munmap(memory_map_, size_);
      memory_map_ = nullptr;
    }
  }
}

