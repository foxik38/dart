#pragma once
#include <type_traits>
#include <string_view>

#include "defs.hpp"
#include "file.hpp"

namespace dart {
  template <typename T>
  concept IsValidStruct = std::is_trivially_copyable_v<T> &&
                          std::is_class_v<T> && !std::is_pointer_v<T>;

  template <TableData D>
  concept IsValidData = D.size >= 4096 && D.size <= (1ULL << 32) &&
                        std::string_view(D.name).contains(".db");

  template <typename T, TableData D, Flags F = {}>
    requires IsValidStruct<T> && IsValidData<D>
  class alignas(64) Table {
   public:
    using table_t = T;

    constexpr explicit Table() {}

    void Open() { file_.emplace(&table_data_, flags_); }

    [[nodiscard]] constexpr TableData& data() noexcept { return table_data_; }

    [[nodiscard]] constexpr Flags flags() const noexcept { return flags_; }

    Table& operator=(const Table&) = delete;
    Table& operator=(Table&&) = delete;
    Table(const Table&) = delete;

    Table(Table&& old) noexcept
        : table_data_{old.table_data_},
          flags_{old.flags_} {
      old.table_data_ = {}, old.flags_ = {};
    }

   private:
    std::optional<File> file_;
    TableData table_data_{D};
    Flags flags_{F};
  };
}