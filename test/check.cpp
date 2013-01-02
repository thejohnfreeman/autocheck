#include <algorithm>
#include <gtest/gtest.h>
#include <autocheck/check.hpp>
#include <autocheck/sequence.hpp>

namespace ac = autocheck;

TEST(Check, Compiles) {
  ac::check<bool>(100, [] (bool b) { return true; },
      ac::make_arbitrary<bool>());
  ac::check<int>(100, [] (int i) { return i >= 0; },
      ac::make_arbitrary<int>());
  //ac::check<bool>(100, [] (bool b) { return true; }); // ICEs Clang

  std::function<bool (const std::vector<int>&)> reverse_prop =
    [] (const std::vector<int>& a) {
      std::vector<int> b(a);
      std::reverse(b.begin(), b.end());
      std::reverse(b.begin(), b.end());
      return a == b;
    };

  ac::check<std::vector<int>>(100, reverse_prop,
      ac::make_arbitrary(ac::list_of<int>()));
  ac::check<std::vector<int>>(100, reverse_prop,
      ac::make_arbitrary(ac::cons<std::vector<int>, unsigned int, int>()));
  ac::check<std::vector<int>>(100, reverse_prop,
      ac::make_arbitrary(ac::ordered_list<int>()));
}

