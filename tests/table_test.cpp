#include <gtest/gtest.h>

#include "file.hpp"

TEST(File, CreatesInstance) {
  const std::array tables{
    dart::File::Create<8000>({"users.db"}),
    dart::File::Create<16000>({"orders.db"})
  };

  for (uint32_t i{0}; i < tables.size(); ++i) {
    EXPECT_EQ(tables[i].page_mode(), dart::file_utils::Page::kStandard);
    EXPECT_GE(tables[i].file_descriptor(), 0);

    unlink(tables[i].c_path());
  }
}