#include <iostream>
#include <gtest/gtest.h>
#include <autocheck/arbitrary.hpp>

namespace ac = autocheck;

TEST(ArbitraryBool, Limited) {
  static const size_t limit = 10;
  ac::arbitrary<bool> gen;
  std::clog << "sizeof(limitable) = " << sizeof(ac::limitable) << std::endl;
  std::clog << "sizeof(arbitrary<bool>) = " << sizeof(gen) << std::endl;
  gen = ac::at_most(limit, gen);

  std::clog << std::boolalpha;
  for (int i = 0; i < limit; ++i) {
    std::clog << gen() << ", ";
  }
  std::clog << std::endl;

  ASSERT_TRUE(gen.exhausted());
}

