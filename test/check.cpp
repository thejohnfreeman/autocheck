#include <algorithm>
#include <gtest/gtest.h>
#include <autocheck/check.hpp>
#include <autocheck/sequence.hpp>

namespace ac = autocheck;

struct reverse_prop_t {
  template <typename T>
  bool operator() (const std::vector<T>& a) const {
    std::vector<T> b(a);
    std::reverse(b.begin(), b.end());
    std::reverse(b.begin(), b.end());
    return a == b;
  }
};

TEST(Check, Compiles) {
  ac::gtest_reporter report;

  ac::check<bool>(100, [] (bool b) { return true; },
      ac::make_arbitrary<bool>(), report);
  ac::check<int>(100, [] (int i) { return i >= 0; },
      ac::make_arbitrary<int>(), report);
  //ac::check<bool>(100, [] (bool b) { return true; }); // ICEs Clang

  reverse_prop_t reverse_prop;

  ac::check<std::vector<int>>(100, reverse_prop,
      ac::make_arbitrary(ac::list_of<int>()), report);
  ac::check<std::vector<int>>(100, reverse_prop,
      ac::make_arbitrary(ac::cons<std::vector<int>, unsigned int, int>()),
      report);

  ac::check<std::vector<char>>(100, reverse_prop,
      ac::make_arbitrary(ac::list_of<char>()), report);

  std::function<bool (const std::string&)> print_prop =
    [] (const std::string& s) { return !!(std::clog << s << std::endl); };

  ac::check<std::string>(100, print_prop,
      ac::make_arbitrary<std::string>(), report);
  ac::check<std::string>(100, print_prop,
      ac::make_arbitrary(ac::make_string_generator<ac::ccPrintable>()),
      report);
}

