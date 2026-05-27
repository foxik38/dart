#pragma once

#include "config.hpp"

namespace dart {
  template <uint64_t S>
  concept IsValidSize =
      S >= 4096 &&
      S <= (1ULL << 32);

  template <Config C>
  concept IsValidConfig =
      IsValidSize<C.size> && std::string_view(C.name).contains(".db");

  class alignas(64) Table {
   public:
    template <Config C>
      requires IsValidConfig<C>
    static Table Construct() {
      return Table(C);
    }

    [[nodiscard]] constexpr std::string_view name() const noexcept {
      return config_.name;
    }

    [[nodiscard]] constexpr uint64_t size() const noexcept {
      return config_.size;
    }

    Table& operator=(const Table&) = delete;
    Table& operator=(Table&&) = delete;
    Table(const Table&) = delete;
    Table(Table&&) = delete;

   private:
    Config config_;

    explicit Table(const Config& config) : config_{config} {}
  };
}