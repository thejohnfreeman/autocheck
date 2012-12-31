#include <iostream>
#include <gtest/gtest.h>
#include <autocheck/arbitrary.hpp>

namespace ac = autocheck;

static const size_t limit = 10;

TEST(Arbitrary, Sizes) {
  std::clog << "sizeof(limitable) = "
    << sizeof(ac::limitable) << std::endl;
  std::clog << "sizeof(arbitrary<bool>) = "
    << sizeof(ac::arbitrary<bool>) << std::endl;
}

TEST(ArbitraryBool, LimitedFreeFunction) {
  ac::arbitrary<bool> gen;
  ac::at_most(limit, gen);

  std::clog << std::boolalpha;
  for (int i = 0; i < limit; ++i) {
    std::clog << gen() << ", ";
  }
  std::clog << std::endl;

  ASSERT_TRUE(gen.exhausted());
}

TEST(ArbitraryBool, LimitedCombinator) {
  auto gen = ac::at_most(limit, ac::arbitrary<bool>());

  std::clog << std::boolalpha;
  for (int i = 0; i < limit; ++i) {
    std::clog << gen() << ", ";
  }
  std::clog << std::endl;

  ASSERT_TRUE(gen.exhausted());
}

TEST(ArbitraryBool, Filtered) {
  auto gen =
    ac::only([] (bool b) { return b; },
     ac::at_most(limit,
       ac::arbitrary<bool>()));
}

