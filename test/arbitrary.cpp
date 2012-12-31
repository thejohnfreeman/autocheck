#include <iostream>
#include <gtest/gtest.h>
#include <autocheck/arbitrary.hpp>

namespace ac = autocheck;

static const size_t limit = 10;

TEST(Arbitrary, Sizes) {
  std::clog << "sizeof(arbitrary<bool>) = "
    << sizeof(ac::arbitrary<bool>) << std::endl;
}

TEST(ArbitraryBool, Generating) {
  auto gen = ac::arbitrary<bool>();

  ac::value<bool> b;
  std::clog << std::boolalpha;
  for (int i = 0; i < limit; ++i) {
    ASSERT_TRUE(gen(b));
    std::clog << b << ", ";
  }
  std::clog << std::endl;

  ASSERT_TRUE(gen(b));
}

TEST(ArbitraryBool, AtMost) {
  auto gen = ac::arbitrary<bool>();
  gen.at_most(limit);

  ac::value<bool> b;
  std::clog << std::boolalpha;
  for (int i = 0; i < limit; ++i) {
    ASSERT_TRUE(gen(b));
    std::clog << b << ", ";
  }
  std::clog << std::endl;

  ASSERT_FALSE(gen(b));
}

TEST(ArbitraryBool, Only) {
  auto gen = ac::arbitrary<bool>();
  gen.only([] (bool b) { return b; });

  ac::value<bool> b;
  std::clog << std::boolalpha;
  for (int i = 0; i < limit; ++i) {
    gen(b);
    ASSERT_TRUE(b);
    std::clog << b << ", ";
  }
  std::clog << std::endl;
}

TEST(ArbitraryBool, Chaining) {
  auto gen = ac::arbitrary<bool>();
  gen.at_most(limit)
     .only([] (bool b) { return b; });

  ac::value<bool> b;
  std::clog << std::boolalpha;
  while (gen(b)) {
    ASSERT_TRUE(b);
    std::clog << b << ", ";
  }
  std::clog << std::endl;

  ASSERT_FALSE(gen(b));
}

TEST(ArbitraryBool, Combinator) {
  auto gen =
    ac::at_most(limit,
        ac::only([] (bool b) { return !b; },
          ac::arbitrary<bool>()));

  ac::value<bool> b;
  std::clog << std::boolalpha;
  while (gen(b)) {
    ASSERT_FALSE(b);
    std::clog << b << ", ";
  }
  std::clog << std::endl;

  ASSERT_FALSE(gen(b));
}

