#pragma once
#include <expected>
#include <limits>
#include <string>
#include <string_view>
#include <utility>

#include "paging.hpp"

namespace dart {
  using error_t = int32_t;

  template <uint64_t N>
  concept IsValidSize =
      std::cmp_greater_equal(N, 4096) &&
      std::cmp_less(
          N,
          static_cast<uint64_t>(std::numeric_limits<uint32_t>::max()) * 4 - 4);

  class File {
   public:
    template <uint64_t N>
      requires IsValidSize<N>
    static File Create(std::string path) {
      return File(std::move(path), N);
    }

    [[nodiscard]] std::string_view path() const noexcept { return path_; }
    [[nodiscard]] const char* c_path() const noexcept { return path_.c_str(); }
    [[nodiscard]] uint64_t size() const noexcept { return size_; }
    [[nodiscard]] paging::Mode page_mode() const noexcept { return page_mode_; }
    [[nodiscard]] uint8_t* memory_map() const noexcept { return memory_map_; }
    [[nodiscard]] int32_t file_descriptor() const noexcept {
      return file_descriptor_;
    }

    File& operator=(const File&) = delete;
    File& operator=(File&&) = delete;
    File(const File&) = delete;
    File(File&&) = delete;

    ~File() { CloseAndUnmapMemory(); }

   private:
    std::string path_{"dart.db"};
    uint64_t size_{4096};
    paging::Mode page_mode_{paging::Mode::kStandard};
    uint8_t* memory_map_{nullptr};
    int32_t file_descriptor_{-1};

    explicit File(std::string&& path, uint64_t size)
        : path_{std::move(path)}, size_{size} {
      HandleResult(Open());
      HandleResult(MapMemory());
    }

    [[nodiscard]] std::expected<void, error_t> Open() noexcept;
    [[nodiscard]] std::expected<void, error_t> MapMemory() noexcept;
    void CloseAndUnmapMemory() noexcept;

    static void HandleResult(std::expected<void, error_t> result) {
      if (!result.has_value())
        throw std::bad_expected_access<error_t>(result.error());
    }
  };
}