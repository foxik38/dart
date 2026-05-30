#pragma once
#include <tuple>

#include "table.hpp"

namespace dart {
  template <typename T>
  concept IsTable = requires(T t) {
    typename T::table_t;
    { t.data() } -> std::same_as<TableData&>;
    { t.flags() } -> std::same_as<Flags>;
  };

  template <typename... T>
  concept IsValidTableCount = sizeof...(T) >= 1 &&
    sizeof...(T) <= std::numeric_limits<uint8_t>::max();

  template <typename... T>
    requires IsValidTableCount<T...> && (IsTable<T> && ...)
  class Database {
   public:
    constexpr explicit Database(T&&... tables)
        : tables_{std::move(tables)...} {}

    template <uint64_t Tt>
    [[nodiscard]] constexpr auto& table() noexcept {
      return std::get<Tt>(tables_);
    }

    Database& operator=(const Database&) = delete;
    Database& operator=(Database&&) = delete;
    Database(const Database&) = delete;
    Database(Database&&) = delete;

   private:
    std::tuple<T...> tables_;
  };
}