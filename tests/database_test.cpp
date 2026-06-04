#include <gtest/gtest.h>

#include "database.hpp"
#include "table.hpp"

TEST(Database, BuildDatabaseAndCheckSize) {
  struct Users {
    char pwd[128];
    char user[32];
    uint64_t id;
  };

  struct Products {
    char id[16];
    char title[32];
    uint64_t price;
  };

  constexpr dart::TableData users_data {"users.db", 5000}, products_data {"products.db", 10000};

  auto db = dart::Database(
    dart::Table<Users, users_data>(),
    dart::Table<Products, products_data>()
  );

  db.Build();

  EXPECT_EQ(sizeof(db), 192 * 2);
}