#include "gtest/gtest.h"

class SampleTest : public ::testing::Test {
};

TEST_F(SampleTest, Sample) {
  int i = 1;

  EXPECT_EQ(1, i);
}


