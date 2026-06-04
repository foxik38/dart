#include "database.hpp"
#include "defs.hpp"
#include "table.hpp"

int main()
{
  struct users {
    char pwd[128];
    char user[32];
    uint64_t id;
  };

  struct products {
    char id[16];
    char title[32];
    uint64_t price;
  };

  constexpr dart::table_data users_data{"users.db", 5000};
  constexpr dart::table_data products_data{"products.db", 10000};

  auto db = dart::database(dart::table<users, users_data>(),
                           dart::table<products, products_data>());

  db.build();
}