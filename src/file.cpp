#include "file.hpp"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>

namespace dart {
  std::expected<void, error_t> File::Open() noexcept {
    config_.size = file_utils::Align(config_.size).first;

    int flags = config_.read_only ? O_RDONLY : (O_RDWR | O_CREAT);
    int permissions = S_IRUSR;

    if (!config_.read_only) permissions |= S_IWUSR;

    int32_t temp_fd = open(config_.name, flags, permissions);
    if (temp_fd == -1) return std::unexpected(errno);

    if (struct stat stat_struct{}; fstat(temp_fd, &stat_struct) != -1) {
      if (stat_struct.st_size == 0 && !config_.read_only) {
        if (ftruncate(temp_fd, static_cast<off_t>(config_.size)) == -1) {
          close(temp_fd);
          return std::unexpected(errno);
        }
        if (fstat(temp_fd, &stat_struct) == -1) return std::unexpected(errno);
      }

      const auto [new_size, page_mode] = file_utils::Align(stat_struct.st_size);

      config_.size = new_size;
      page_mode_ = page_mode;
      file_descriptor_ = temp_fd;

      return {};
    }
    close(temp_fd);
    return std::unexpected(errno);
  }

  std::expected<void, error_t> File::MapMemory() noexcept {
    if (!memory_map_) {
      int mem_protection = PROT_READ;

      if (!config_.read_only) mem_protection |= PROT_WRITE;

      void* temp_mmap = mmap(nullptr, config_.size, mem_protection, MAP_SHARED,
                             file_descriptor_, 0);

      if (temp_mmap == MAP_FAILED) return std::unexpected<error_t>(errno);

      memory_map_ = static_cast<uint8_t*>(temp_mmap);

      if (page_mode_ == file_utils::Page::kHuge) {
        if (madvise(memory_map_, config_.size, MADV_HUGEPAGE) == -1)
          return std::unexpected(errno);
      }
    }
    return {};
  }

  void File::CloseAndUnmapMemory() noexcept {
    if (memory_map_) {
      msync(memory_map_, config_.size, MS_SYNC);
      munmap(memory_map_, config_.size);
      memory_map_ = nullptr;
    }

    if (file_descriptor_ != -1) {
      close(file_descriptor_);
      file_descriptor_ = -1;
    }
  }
}
