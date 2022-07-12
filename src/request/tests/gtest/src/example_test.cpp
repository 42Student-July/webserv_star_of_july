#include <gtest/gtest.h>

int increment(int x);

TEST(example_est, func_increment) {
  ASSERT_EQ(1, increment(0));
  ASSERT_EQ(2, increment(1));
}
