#pragma once
#include <tuple>

#include "file.hpp"
#include "constraints.hpp"

namespace dart {
  template <typename... T>
    requires IsValidTableCount<T...> && (IsTable<T> && ...)
  class Database {
   public:
    constexpr explicit Database(T&&... tables) : tables_{std::move(tables)...} {}

    void Build() {
      std::apply([](auto&... table) { (table.Open(), ...); }, tables_);
    }

    template <uint64_t I>
    [[nodiscard]] constexpr auto& table() noexcept {
      return std::get<I>(tables_);
    }

    Database& operator=(const Database&) = delete;
    Database& operator=(Database&&) = delete;
    Database(const Database&) = delete;
    Database(Database&&) = delete;

   private:
    std::tuple<T...> tables_;
  };
}