#include "file.hpp"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace dart {

  std::expected<void, error_t> File::VerifyConfig() const noexcept {
    if (config_.path.empty() || !config_.path.ends_with(".db") ||
        config_.path.contains("/") || (config_.read_only && config_.override))
      return std::unexpected(EINVAL);

    return {};
  }

  std::expected<void, error_t> File::Open() noexcept {
    int flags = config_.read_only ? O_RDONLY : O_RDWR;
    int permissions = S_IRUSR;

    flags |= O_CREAT;

    if (!config_.read_only) {
      permissions |= S_IWUSR;

      if (config_.override) flags |= O_TRUNC;
    }

    int32_t temp_fd = open(config_.path.c_str(), flags, permissions);
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

  std::expected<void, error_t> File::MapMemory() noexcept {
    if (!memory_map_) {
      int mem_protection = PROT_READ;

      if (!config_.read_only) mem_protection |= PROT_WRITE;

      void* temp_mmap =
          mmap(nullptr, size_, mem_protection, MAP_SHARED, file_descriptor_, 0);

      if (temp_mmap == MAP_FAILED) return std::unexpected<error_t>(errno);

      memory_map_ = static_cast<uint8_t*>(temp_mmap);

      if (page_mode_ == file_utils::Page::kHuge) {
        if (madvise(memory_map_, size_, MADV_HUGEPAGE) == -1)
          return std::unexpected(errno);
      }
    }
    return {};
  }

  std::expected<void, error_t> File::Sync() noexcept {
    if (config_.sync_on != 0) {
      if (write_counter_ == config_.sync_on) {
        if (msync(memory_map_, size_, MS_SYNC) == -1)
          return std::unexpected(errno);

        write_counter_ = 0;
      }
    } return {};
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
