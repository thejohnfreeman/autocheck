#include <algorithm>
#include <gtest/gtest.h>
#include <autocheck/sequence.hpp>
#include <autocheck/check.hpp>

namespace ac = autocheck;

struct reverse_prop_t {
  template <typename Container>
  bool operator() (Container& xs) const {
    Container ys(xs);
    std::reverse(xs.begin(), xs.end());
    std::reverse(xs.begin(), xs.end());
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

TEST(Check, Compiles) {
  ac::gtest_reporter rep;

  ac::check<bool>([] (bool x) { return true; }, 100,
      ac::make_arbitrary<bool>(), rep);

  /* This tests that every signed integer is positive. It is intended to
   * demonstrate failure behavior. */
  ac::check<int>([] (int x) { return x >= 0; }, 100,
      ac::make_arbitrary<int>(), rep);

  //ac::check<bool>([] (bool x) { return true; }); // ICEs Clang

  reverse_prop_t reverse_prop;

  ac::check<std::vector<int>>(reverse_prop, 100,
      ac::make_arbitrary(ac::list_of<int>()), rep);
  ac::check<std::vector<int>>(reverse_prop, 100,
      ac::make_arbitrary(ac::cons<std::vector<int>, unsigned int, int>()),
      rep);

  ac::check<std::vector<char>>(reverse_prop, 100,
      ac::make_arbitrary(ac::list_of(ac::generator<char>())), rep);
  ac::check<std::vector<std::string>>(reverse_prop, 100,
      ac::make_arbitrary(ac::list_of<std::string>()), rep);

  ac::check<std::string>(reverse_prop, 100,
      ac::make_arbitrary<std::string>(), rep);
  ac::check<std::string>(reverse_prop, 100,
      ac::make_arbitrary(ac::string<>()),
      rep);

  /* Chaining, ... */
  //auto arb = ac::make_arbitrary(ac::generator<int>(), ac::ordered_list<int>())
    //.discard_if([] (int, const std::vector<int>& xs) -> bool { return !std::is_sorted(xs.begin(), xs.end()); })
    //.discard_at_most(100);

  /* ... or combinators. */
  auto arb =
    ac::discard_if([] (int, const std::vector<int>& xs) -> bool { return !std::is_sorted(xs.begin(), xs.end()); },
    ac::discard_at_most(100,
    //ac::make_arbitrary(ac::generator<int>(), ac::ordered_list<int>())));
    ac::make_arbitrary<int, std::vector<int>>()));
    //ac::make_arbitrary(ac::generator<int>(), ac::list_of<int>())));

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

  ac::check<int, std::vector<int>>(
      [] (int x, std::vector<int>& xs) -> bool {
        insert_sorted(x, xs);
        return std::is_sorted(xs.begin(), xs.end());
      },
      100, ac::copy(arb), rep, ac::copy(cls));
}

