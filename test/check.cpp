#include <algorithm>
#include <gtest/gtest.h>
#include <autocheck/check.hpp>
#include <autocheck/sequence.hpp>

namespace ac = autocheck;

struct reverse_prop_t {
  template <typename T>
  bool operator() (const std::vector<T>& xs) const {
    std::vector<T> ys(xs);
    std::reverse(ys.begin(), ys.end());
    std::reverse(ys.begin(), ys.end());
    return xs == ys;
  }
};

template <typename T>
void insert_sorted(const T& x, std::vector<T>& xs) {
  xs.push_back(x);
  if (xs.size() == 1) return;
  for (std::vector<int>::reverse_iterator
      b = xs.rbegin(), a = b++, e = xs.rend(); (b != e) && (*a < *b); ++b, ++a)
  {
    std::iter_swap(a, b);
  }
}

template <typename T>
T copy(const T& t) { return t; }

TEST(Check, Compiles) {
  ac::gtest_reporter rep;

  ac::check<bool>(100, [] (bool x) { return true; },
      ac::make_arbitrary<bool>(), copy(rep));
  ac::check<int>(100, [] (int x) { return x >= 0; },
      ac::make_arbitrary<int>(), copy(rep));
  //ac::check<bool>(100, [] (bool x) { return true; }); // ICEs Clang

  reverse_prop_t reverse_prop;

  ac::check<std::vector<int>>(100, reverse_prop,
      ac::make_arbitrary(ac::list_of<int>()), copy(rep));
  ac::check<std::vector<int>>(100, reverse_prop,
      ac::make_arbitrary(ac::cons<std::vector<int>, unsigned int, int>()),
      copy(rep));

  ac::check<std::vector<char>>(100, reverse_prop,
      ac::make_arbitrary(ac::list_of(ac::generator<char>())), copy(rep));
  ac::check<std::vector<std::string>>(100, reverse_prop,
      ac::make_arbitrary(ac::list_of<std::string>()), copy(rep));

  //std::function<bool (const std::string&)> print_prop =
    //[] (const std::string& x) { return !!(std::clog << x << std::endl); };

  //ac::check<std::string>(100, print_prop,
      //ac::make_arbitrary<std::string>(), copy(rep));
  //ac::check<std::string>(100, print_prop,
      //ac::make_arbitrary(ac::make_string_generator<ac::ccPrintable>()),
      //copy(rep));

  /* Chaining, ... */
  //auto arb = ac::make_arbitrary(ac::generator<int>(), ac::ordered_list<int>())
    //.only_if([] (int, const std::vector<int>& xs) -> bool { return std::is_sorted(xs.begin(), xs.end()); })
    //.at_most(100);

  /* ... or combinators. */
  auto arb =
    ac::at_most(100,
    ac::only_if([] (int, const std::vector<int>& xs) -> bool { return std::is_sorted(xs.begin(), xs.end()); },
    //ac::make_arbitrary(ac::generator<int>(), ac::ordered_list<int>())));
    ac::make_arbitrary(ac::generator<int>(), ac::list_of<int>())));

  ac::classifier<int, std::vector<int>> cls;
  cls.trivial([] (int, const std::vector<int>& xs) { return xs.empty(); });
  cls.collect([] (int x, const std::vector<int>& xs) { return xs.size(); });
  cls.classify([] (int x, const std::vector<int>& xs) { return xs.empty() || (x < xs.front()); }, "at-head");
  cls.classify([] (int x, const std::vector<int>& xs) { return xs.empty() || (xs.back() < x); }, "at-tail");

  /* Can't use combinators here because it breaks template deduction (?). */
  //ac::classifier<int, std::vector<int>> cls(
    //ac::trivial([] (int, const std::vector<int>& xs) { return xs.empty(); },
    //ac::collect([] (int x, const std::vector<int>& xs) { return xs.size(); },
    //ac::classify([] (int x, const std::vector<int>& xs) { return xs.empty() || (x < xs.front()); }, "at-head",
    //ac::classify([] (int x, const std::vector<int>& xs) { return xs.empty() || (xs.back() < x); }, "at-tail",
    //ac::classifier<int, const std::vector<int>>()))));

  ac::check<int, std::vector<int>>(100,
      [] (int x, const std::vector<int>& xs) -> bool {
        std::vector<int> ys(xs);
        insert_sorted(x, ys);
        return std::is_sorted(ys.begin(), ys.end());
      },
      copy(arb), copy(rep), copy(cls));
}

