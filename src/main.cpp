#include <iostream>

#include "database.hpp"
#include "file.hpp"
#include "table.hpp"

int main() {
  constexpr dart::Config cfg {"orders.db", 4096 * 2};

  struct Users {
    char pwd[128];
    char user[32];
    uint64_t id;
  };

  auto users = dart::Table<Users, {"users.db", 4096}>();

  std::cout << "users: " << users.size() << '\n'; // 4096 (size)

  auto users_new = std::move(users);

  std::cout << "users: " << users.size() << '\n'; // 0 (MOVED)
  std::cout << "users_new: " << users_new.size() << '\n'; // 4096 (MOVED)
}