#include "file.hpp"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>

namespace dart {

  std::expected<void, error_t> File::Open() noexcept {
    if (path().empty() || !path().ends_with(".db") || path().contains("/"))
      return std::unexpected(EINVAL);

    int32_t temp_fd = open(c_path(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (temp_fd == -1) return std::unexpected(errno);

    if (struct stat stat_struct{}; fstat(temp_fd, &stat_struct) != -1) {
      if (stat_struct.st_size == 0) {
        if (ftruncate(temp_fd, static_cast<__off_t>(size_)) == -1) {
          close(temp_fd);
          return std::unexpected(errno);
        }
        fstat(temp_fd, &stat_struct);
      }

      const auto [size, page_mode] = paging::Align(stat_struct.st_size);

      size_ = size;
      page_mode_ = page_mode;
      file_descriptor_ = temp_fd;

      return {};
    }
    close(temp_fd);
    return std::unexpected(errno);
  }

  std::expected<void, error_t> File::MapMemory() noexcept { return {}; }

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
}  // namespace dart
