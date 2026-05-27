#include <gtest/gtest.h>

#include "table.hpp"

TEST(Table, CheckTableSize) {
  const std::array tables {
    dart::Table::Construct<{"users.db", 4096}>(),
    dart::Table::Construct<{"orders.db", 4096}>(),
    dart::Table::Construct<{"customers.db", 4096}>(),
    dart::Table::Construct<{"salaries.db", 4096}>(),
    dart::Table::Construct<{"logs.db", 4096}>()
  };

  for (auto& table: tables) {
    EXPECT_EQ(table.size(), 4096);
  }

  EXPECT_EQ(sizeof(tables), 5 * 64);
}