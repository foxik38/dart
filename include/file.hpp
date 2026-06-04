#pragma once
#include <expected>

#include "defs.hpp"
#include "file_utils.hpp"

namespace dart
{
  using error_t = int32_t;

  class alignas(64) file
  {
  public:
    explicit file(table_data& table, table_flags flags)
        : data_{table}, flags_{flags}
    {
      data_.size = file_utils::align(data_.size).first;
      handle_result(open().and_then([&] { return map(); }));
    }

    file& operator=(const file&) = delete;
    file& operator=(file&&) = delete;
    file(const file&) = delete;
    file(file&& old) = delete;

    ~file() { close_and_unmap(); }

  private:
    table_data& data_;
    const table_flags flags_;
    file_utils::page page_mode_{file_utils::page::standard};
    uint8_t* memory_map_{nullptr};
    int32_t file_descriptor_{-1};

    [[nodiscard]] std::expected<void, error_t> open() noexcept;
    [[nodiscard]] std::expected<void, error_t> map() noexcept;
    void close_and_unmap() noexcept;

    static void handle_result(std::expected<void, error_t> result)
    {
      if (!result.has_value())
        throw std::bad_expected_access<error_t>(result.error());
    }
  };
} // namespace dart