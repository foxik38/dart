#include "file.hpp"

#include <cerrno>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace dart
{
  std::expected<void, error_t> file::open() noexcept
  {
    int flags = flags_.read_only ? O_RDONLY : (O_RDWR | O_CREAT);
    int permissions = S_IRUSR;

    if (!flags_.read_only) {
      permissions |= S_IWUSR;
    }

    int32_t temp_fd = ::open(data_.name, flags, permissions);
    if (temp_fd == -1) {
      return std::unexpected(errno);
    }

    if (struct stat stat_struct{}; fstat(temp_fd, &stat_struct) != -1) {
      if (stat_struct.st_size == 0 && !flags_.read_only) {
        if (ftruncate(temp_fd, static_cast<off_t>(data_.size)) == -1) {
          close(temp_fd);
          return std::unexpected(errno);
        }
        if (fstat(temp_fd, &stat_struct) == -1) {
          close(temp_fd);
          return std::unexpected(errno);
        }
      }

      const auto [new_size, page_mode] = file_utils::align(stat_struct.st_size);

      data_.size = new_size;
      page_mode_ = page_mode;
      file_descriptor_ = temp_fd;

      return {};
    }
    ::close(temp_fd);
    return std::unexpected(errno);
  }

  std::expected<void, error_t> file::map() noexcept
  {
    if (memory_map_ == nullptr) {
      int mem_protection = PROT_READ;

      if (!flags_.read_only) {
        mem_protection |= PROT_WRITE;
      }

      void* temp_mmap = mmap(nullptr, data_.size, mem_protection, MAP_SHARED,
                             file_descriptor_, 0);

      if (temp_mmap == MAP_FAILED) {
        return std::unexpected<error_t>(errno);
      }

      memory_map_ = static_cast<uint8_t*>(temp_mmap);

      if (page_mode_ == file_utils::page::huge) {
        if (madvise(memory_map_, data_.size, MADV_HUGEPAGE) == -1) {
          munmap(memory_map_, data_.size);
          memory_map_ = nullptr;
          return std::unexpected(errno);
        }
      }
    }
    return {};
  }

  void file::close_and_unmap() noexcept
  {
    if (memory_map_ != nullptr) {
      msync(memory_map_, data_.size, MS_SYNC);
      munmap(memory_map_, data_.size);
      memory_map_ = nullptr;
    }

    if (file_descriptor_ != -1) {
      close(file_descriptor_);
      file_descriptor_ = -1;
    }
  }
} // namespace dart
