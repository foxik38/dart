#pragma once
#include <tuple>

#include "constraints.hpp"
#include "file.hpp"

namespace dart
{
  template <typename... T>
    requires valid_tables<T...> && (valid_table<T> && ...)
  class database
  {
  public:
    constexpr explicit database(T&&... tables) : tables_{std::move(tables)...}
    {
    }

    void build()
    {
      std::apply([](auto&... table) { (table.open(), ...); }, tables_);
    }

    template <uint64_t I> [[nodiscard]] constexpr auto& table() noexcept
    {
      return std::get<I>(tables_);
    }

    database& operator=(const database&) = delete;
    database& operator=(database&&) = delete;
    database(const database&) = delete;
    database(database&&) = delete;

  private:
    std::tuple<T...> tables_;
  };
} // namespace dart