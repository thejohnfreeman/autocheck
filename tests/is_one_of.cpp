#include <autocheck/is_one_of.hpp>

#include <gtest/gtest.h>

namespace ac = autocheck;

TEST(IsOneOf, Same) {
  ASSERT_TRUE((ac::is_one_of<char, char>::value));
}

TEST(IsOneOf, DecayReference) {
  ASSERT_TRUE((ac::is_one_of<char&, char>::value));
  ASSERT_TRUE((ac::is_one_of<char, char&>::value));
}

TEST(IsOneOf, DecayArray) {
  ASSERT_TRUE((ac::is_one_of<char[], char*>::value));
  ASSERT_TRUE((ac::is_one_of<char*, char[]>::value));
}

TEST(IsOneOf, Signed) {
  ASSERT_TRUE((ac::is_one_of<signed int, int>::value));
  ASSERT_TRUE((ac::is_one_of<int, signed int>::value));
}

TEST(IsOneOf, Unsigned) {
  ASSERT_FALSE((ac::is_one_of<unsigned int, int>::value));
  ASSERT_FALSE((ac::is_one_of<int, unsigned int>::value));
}

TEST(IsOneOf, SearchSuccess) {
  ASSERT_TRUE((ac::is_one_of<int, char, int, long>::value));
}

TEST(IsOneOf, SearchFailure) {
  ASSERT_FALSE((ac::is_one_of<float, char, int, long>::value));
}

