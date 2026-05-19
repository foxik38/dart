#include <gtest/gtest.h>

#include "paging.hpp"

TEST(Paging, AlignsStandardPages) {
  constexpr uint64_t bytes {5000};
  const auto [size, mode] = dart::paging::Align(bytes);

  EXPECT_EQ(size, 4096 * 2);
  EXPECT_EQ(mode, dart::paging::Mode::kStandard);
}

TEST(Paging, AlignsHugePages) {
  constexpr uint64_t bytes {2 * 1024 * 1024};
  const auto [size, mode] = dart::paging::Align(bytes + 1024);

  EXPECT_EQ(size, bytes * 2);
  EXPECT_EQ(mode, dart::paging::Mode::kHuge);
}