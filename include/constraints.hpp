#pragma once
#include <string_view>
#include <type_traits>

#include "defs.hpp"

namespace dart {
  template <typename T>
  concept IsValidStruct =
      std::is_standard_layout_v<T> && std::is_trivially_copyable_v<T> &&
      std::is_class_v<T> && !std::is_pointer_v<T>;

  template <TableData D>
  concept IsValidData = D.size >= 4096 && D.size <= (1ULL << 32) &&
                        std::string_view(D.name).contains(".db");

  template <typename T>
  concept IsTable = requires(T t) {
    typename T::table_t;
    { t.data() } -> std::same_as<TableData&>;
    { t.flags() } -> std::same_as<Flags>;
  };

  template <typename... T>
  concept IsValidTableCount =
      sizeof...(T) >= 1 && sizeof...(T) <= std::numeric_limits<uint8_t>::max();
} // namespace dart