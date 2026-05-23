#include <gtest/gtest.h>

#include "file_utils.hpp"

TEST(Paging, AlignsStandardPages) {
  constexpr uint64_t bytes {5000};
  const auto [size, mode] = dart::file_utils::Align(bytes);

  EXPECT_EQ(size, 4096 * 2);
  EXPECT_EQ(mode, dart::file_utils::Page::kStandard);
}

TEST(Paging, AlignsHugePages) {
  constexpr uint64_t bytes {2 * 1024 * 1024};
  const auto [size, mode] = dart::file_utils::Align(bytes + 1024);

  EXPECT_EQ(size, bytes * 2);
  EXPECT_EQ(mode, dart::file_utils::Page::kHuge);
}