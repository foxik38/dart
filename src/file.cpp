#include "file.hpp"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace dart {

  std::expected<void, error_t> File::Open() noexcept {
    if (config_.path.empty() || !config_.path.ends_with(".db") ||
        config_.path.contains("/"))
      return std::unexpected(EINVAL);

    int flags = config_.read_only ? O_RDONLY : O_RDWR;
    int permissions = S_IRUSR;

    flags |= O_CREAT;

    if (config_.read_only && config_.override)
      return std::unexpected(EINVAL);

    if (!config_.read_only) {
      permissions |= S_IWUSR;

      if (config_.override)
        flags |= O_TRUNC;
    }

    int32_t temp_fd =
        open(config_.path.c_str(), flags, permissions);
    if (temp_fd == -1) return std::unexpected(errno);

    if (struct stat stat_struct{}; fstat(temp_fd, &stat_struct) != -1) {
      if (stat_struct.st_size == 0) {
        if (ftruncate(temp_fd, static_cast<__off_t>(size_)) == -1) {
          close(temp_fd);
          return std::unexpected(errno);
        }
        fstat(temp_fd, &stat_struct);
      }

      const auto [new_size, page_mode] = file_utils::Align(stat_struct.st_size);

      size_ = new_size;
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
