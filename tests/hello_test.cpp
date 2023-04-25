#include <gtest.h>

TEST(MultiplyTests, TestIntegerOne_One) {
  const auto expected = 1;
  const auto actual = 1;
  ASSERT_EQ(expected, actual);
}
