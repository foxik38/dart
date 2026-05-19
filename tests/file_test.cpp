#include <gtest/gtest.h>

#include "file.hpp"

TEST(File, CreatesInstance) {
  auto users = dart::File::Create<8000>("users.db");
  auto orders = dart::File::Create<16000>("orders.db");

  EXPECT_EQ(users.size(), 4096 * 2);
  EXPECT_EQ(orders.size(), 4096 * 4);
}