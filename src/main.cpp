#include <iostream>

#include "database.hpp"
#include "file.hpp"
#include "table.hpp"

int main() {
  constexpr dart::Config cfg {"users.db", 4096};
  auto copy_cfg = cfg;

  std::array tables {
    dart::Table::Construct<cfg>(),
    dart::Table::Construct<{"orders.db", 8192}>()
  };

  for (auto& table: tables) {
    std::cout << table.size() << '\n';
  }
}