#include <gtest/gtest.h>
#include <autocheck/largest.hpp>

namespace ac = autocheck;

TEST(Largest, One) {
  ASSERT_EQ(sizeof(char), ac::largest<char>::size);
}

TEST(Largest, Two) {
  ASSERT_EQ(sizeof(int), (ac::largest<char, int>::size));
}

TEST(Largest, Three) {
  ASSERT_EQ(sizeof(long), (ac::largest<char, long, int>::size));
}

TEST(Largest, Four) {
  ASSERT_EQ(sizeof(long long),
      (ac::largest<char, int, long long, int>::size));
}

