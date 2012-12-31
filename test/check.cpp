#include <gtest/gtest.h>
#include <autocheck/check.hpp>

namespace ac = autocheck;

TEST(Check, Compiles) {
  ac::check<bool>(100, [] (bool b) { return true; },
      ac::arbitrary<std::tuple<bool>>());
  //ac::check<bool>(100, [] (bool b) { return true; }); // ICEs Clang
}

