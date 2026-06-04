#pragma once
#include <optional>
#include <cassert>

#include "defs.hpp"
#include "file.hpp"
#include "constraints.hpp"

namespace dart {
  template <typename T, TableData D, Flags F = {}>
    requires IsValidStruct<T> && IsValidData<D>
  class alignas(64) Table {
   public:
    using table_t = T;

    constexpr explicit Table() = default;

    void Open() { file_.emplace(&table_data_, flags_); }

    [[nodiscard]] constexpr TableData& data() noexcept { return table_data_; }

    [[nodiscard]] constexpr Flags flags() const noexcept { return flags_; }

    Table& operator=(const Table&) = delete;
    Table& operator=(Table&&) = delete;
    Table(const Table&) = delete;

    Table(Table&& old) noexcept
        : table_data_{old.table_data_},
          flags_{old.flags_} {
      assert(!old.file_.has_value());
      old.table_data_ = {}, old.flags_ = {};
    }

   private:
    std::optional<File> file_;
    TableData table_data_{D};
    Flags flags_{F};
  };
}