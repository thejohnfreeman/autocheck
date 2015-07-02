#include <algorithm>
#include <gtest/gtest.h>
#include <autocheck/sequence.hpp>
#include <autocheck/check.hpp>

namespace ac = autocheck;

TEST(Check, DemonstrateFailingTest)
{
  ac::gtest_reporter rep;
  /* This tests that every signed integer is positive. It is intended to
   * demonstrate failure behavior. */
  ac::check<int>([] (int x) { return x >= 0; }, 100,
      ac::make_arbitrary<int>(), rep);
}
