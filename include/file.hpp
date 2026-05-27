#pragma once
#include <expected>

#include "config.hpp"
#include "file_utils.hpp"

namespace dart {
  using error_t = int32_t;

  class alignas(32) File {
   public:
    explicit File(Config& config) : config_{config} {
      HandleResult(Open().and_then([&] { return MapMemory(); }));
    }

    File& operator=(const File&) = delete;
    File& operator=(File&&) = delete;
    File(const File&) = delete;
    File(File&& old) = delete;

    ~File() { CloseAndUnmapMemory(); }

   private:
    Config& config_;
    file_utils::Page page_mode_{file_utils::Page::kStandard};
    uint8_t* memory_map_{nullptr};
    int32_t file_descriptor_{-1};

    [[nodiscard]] std::expected<void, error_t> Open() noexcept;
    [[nodiscard]] std::expected<void, error_t> MapMemory() noexcept;
    void CloseAndUnmapMemory() noexcept;

    static void HandleResult(std::expected<void, error_t> result) {
      if (!result.has_value())
        throw std::bad_expected_access<error_t>(result.error());
    }
  };
}