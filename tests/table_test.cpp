#include <gtest/gtest.h>

#include "table.hpp"

TEST(Table, CheckTableStructType) {
  struct EmptyTable {};
  constexpr dart::TableData data {"empty_table.db", 4096};
  bool check_struct = std::is_same_v<dart::Table<EmptyTable, data>::table_t, EmptyTable>;

  EXPECT_TRUE(check_struct);
}

TEST(Table, MoveTableAndCheckTableData) {
  struct Users {};
  constexpr dart::TableData data {"test.db", 4096};
  dart::Table<Users, data> table;
  auto moved_table{std::move(table)};

  EXPECT_NE(&table, &moved_table);

  EXPECT_EQ(table.data().size, 0);

  EXPECT_STREQ(moved_table.data().name, data.name);
  EXPECT_EQ(moved_table.data().size, data.size);
}