#include <iostream>

#include "file.hpp"

int main() {
  try {
    const dart::FileConfig config {
      .path = "dart.db",
      .read_only = false,
      .override = true
    };

    const dart::File db = dart::File::Create<100'000>(config);

    std::cout << "path: " << db.path() << '\n';
    std::cout << "size: " << db.size() << '\n';
    std::cout << "page_mode: " << std::to_underlying(db.page_mode()) << '\n';
    std::cout << "pages in total: " << db.size() / std::to_underlying(db.page_mode()) << '\n';
  } catch (const std::bad_expected_access<dart::error_t>& e) {
    std::cerr << e.error();
  }
}