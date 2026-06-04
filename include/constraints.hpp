#pragma once
#include <string_view>
#include <type_traits>
#include <limits>

#include "defs.hpp"

namespace dart
{
  template <typename T>
  concept valid_struct =
      std::is_standard_layout_v<T> && std::is_trivially_copyable_v<T> &&
      std::is_class_v<T> && !std::is_pointer_v<T>;

  template <table_data T>
  concept valid_table_data = T.size >= 4096 && T.size <= (1ULL << 32) &&
                       std::string_view(T.name).contains(".db");

  template <typename T>
  concept valid_table = requires(T type) {
    typename T::table_type;
    { type.data() } -> std::same_as<table_data&>;
    { type.flags() } -> std::same_as<table_flags>;
  };

  template <typename... T>
  concept valid_tables =
      sizeof...(T) >= 1 && sizeof...(T) <= std::numeric_limits<uint8_t>::max();
} // namespace dart