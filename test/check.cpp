#include <gtest/gtest.h>
#include <autocheck/check.hpp>

namespace ac = autocheck;

TEST(Check, Compiles) {
  ac::check<bool>(100, [] (bool b) { return true; },
      ac::make_arbitrary<bool>());
  ac::check<int>(100, [] (int i) { return i >= 0; },
      ac::make_arbitrary<int>());
  //ac::check<bool>(100, [] (bool b) { return true; }); // ICEs Clang
}

