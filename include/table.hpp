#pragma once
#include <cassert>
#include <optional>

#include "constraints.hpp"
#include "defs.hpp"
#include "file.hpp"

namespace dart
{
  template <typename T, table_data D, table_flags F = {}>
    requires valid_struct<T> && valid_table_data<D>
  class alignas(64) table
  {
  public:
    using table_type = T;

    constexpr explicit table() = default;

    void open() { file_.emplace(data_, flags_); }

    [[nodiscard]] constexpr table_data& data() noexcept { return data_; }

    [[nodiscard]] constexpr table_flags flags() const noexcept { return flags_; }

    table& operator=(const table&) = delete;
    table& operator=(table&&) = delete;
    table(const table&) = delete;

    table(table&& old) noexcept
        : data_{old.data_}, flags_{old.flags_}
    {
      assert(!old.file_.has_value());
      old.data_ = {}, old.flags_ = {};
    }

  private:
    std::optional<file> file_;
    table_data data_{D};
    table_flags flags_{F};
  };
} // namespace dart