#pragma once

#include "config.hpp"

namespace dart {
  template <typename T>
  concept IsValidStruct = std::is_trivially_copyable_v<T> && std::is_class_v<T>;

  template <Config C>
  concept IsValidConfig = C.size >= 4096 && C.size <= (1ULL << 32) &&
                          std::string_view(C.name).contains(".db");

  template <typename T, Config C>
    requires IsValidStruct<T> && IsValidConfig<C>
  class alignas(64) Table {
   public:
    using table_t = T;

    constexpr explicit Table() {}

    [[nodiscard]] constexpr std::string_view name() const noexcept {
      return config_.name;
    }

    [[nodiscard]] constexpr uint64_t size() const noexcept {
      return config_.size;
    }

    Table& operator=(const Table&) = delete;
    Table& operator=(Table&&) = delete;
    Table(const Table&) = delete;

    Table(Table&& old) : config_{std::move(old.config_)} { old.config_ = {}; }

   private:
    Config config_{C};
  };
}