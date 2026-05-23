#pragma once
#include <expected>
#include <limits>
#include <string>
#include <string_view>
#include <utility>

#include "file_utils.hpp"

namespace dart {
  using error_t = int32_t;

  struct FileConfig {
    std::string path;
    std::string directory{"/"};
    bool read_only{false};
    bool override{false};
  };

  template <uint64_t T>
  concept IsValidSize =
      T >= 4096 &&
      T < static_cast<uint64_t>(std::numeric_limits<uint32_t>::max()) * 4 - 4;

  class alignas(64) File {
   public:
    template <uint64_t T>
      requires IsValidSize<T>
    static File Create(FileConfig config) {
      return File(std::move(config), T);
    }

    [[nodiscard]] uint64_t size() const noexcept { return size_; }

    [[nodiscard]] std::string_view path() const noexcept {
      return config_.path;
    }

    [[nodiscard]] const char* c_path() const noexcept {
      return config_.path.c_str();
    }

    [[nodiscard]] file_utils::Page page_mode() const noexcept {
      return page_mode_;
    }

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
    FileConfig config_;
    uint64_t size_;
    file_utils::Page page_mode_{file_utils::Page::kStandard};
    uint8_t* memory_map_{nullptr};
    int32_t file_descriptor_{-1};

    explicit File(FileConfig&& config, uint64_t size)
        : config_{std::move(config)}, size_{size} {
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