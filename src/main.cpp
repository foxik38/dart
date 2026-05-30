#include <iostream>

#include "defs.hpp"
#include "database.hpp"
#include "table.hpp"

int main() {
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

  constexpr dart::TableData users_data {"users.db", 4096};
  constexpr dart::TableData products_data {"products.db", 4096 * 2};

  auto users = dart::Table<Users, users_data>();
  auto products = dart::Table<Products, products_data>();

  dart::Database(std::move(users), std::move(products));

  //OR

  dart::Database(
    dart::Table<Users, users_data>(),
    dart::Table<Products, products_data>()
  );
}