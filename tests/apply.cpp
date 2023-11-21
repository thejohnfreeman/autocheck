#include <limits>
#include <gtest/gtest.h>
#include <autocheck/apply.hpp>

bool f1(char c) {
  return c == 'c';
}

bool f2(char c, const char* p) {
  return (c == 'c') && (p == nullptr);
}

bool f3(char c, const char* p, float f) {
  return (c == 'c') && (p == nullptr) &&
    ((f - 3.14) < std::numeric_limits<float>::epsilon());
}

#ifndef AUTOCHECK_HAS_STD_APPLY

TEST(Apply, One) {
  ASSERT_TRUE(autocheck::apply(f1, std::make_tuple('c')));
}

TEST(Apply, Two) {
  ASSERT_TRUE(autocheck::apply(f2, std::make_tuple('c', nullptr)));
}

TEST(Apply, Three) {
  ASSERT_TRUE(autocheck::apply(f3, std::make_tuple('c', nullptr, 3.14f)));
}

#endif
