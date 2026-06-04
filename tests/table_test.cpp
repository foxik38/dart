#include <gtest/gtest.h>

#include "table.hpp"

TEST(Table, CheckTableStructType) {
  struct empty_table {};
  constexpr dart::table_data data {"empty_table.db", 4096};
  bool check_struct = std::is_same_v<dart::table<empty_table, data>::table_type, empty_table>;

  EXPECT_TRUE(check_struct);

  unlink(data.name);
}

TEST(Table, MoveTableAndCheckTableData) {
  struct users {};
  constexpr dart::table_data data {"test.db", 4096};
  dart::table<users, data> table;
  auto moved_table{std::move(table)};

  EXPECT_NE(&table, &moved_table);

  EXPECT_EQ(table.data().size, 0);

  EXPECT_STREQ(moved_table.data().name, data.name);
  EXPECT_EQ(moved_table.data().size, data.size);

  unlink(moved_table.data().name);
}