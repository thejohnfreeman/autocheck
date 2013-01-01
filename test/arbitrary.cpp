#include <iostream>
#include <gtest/gtest.h>
#include <autocheck/arbitrary.hpp>

namespace ac = autocheck;

static const size_t limit = 10;

template <size_t I, typename... Ts>
void print(std::ostream& out, const std::tuple<Ts...>& tup,
    const std::integral_constant<size_t, I>& = std::integral_constant<size_t, I>())
{
  out << std::get<I>(tup);
  out << ", ";
}

template <typename... Ts>
void print(std::ostream& out, const std::tuple<Ts...>& tup, const std::integral_constant<size_t, 0>&) {
  out << std::get<0>(tup);
}

namespace std {

  template <typename... Ts>
  std::ostream& operator<< (std::ostream& out, const std::tuple<Ts...>& tup) {
    out << "(";
    print(out, tup, std::integral_constant<size_t, sizeof...(Ts) - 1>());
    out << ")";
    return out;
  }

}

TEST(ArbitraryBool, Generating) {
  auto arb = ac::make_arbitrary<bool>();

  ac::value<std::tuple<bool>> b;
  std::clog << std::boolalpha;
  for (int i = 0; i < limit; ++i) {
    ASSERT_TRUE(arb(b));
    if (i > 0) std::clog << ", ";
    std::clog << b;
  }
  std::clog << std::endl;

  ASSERT_TRUE(arb(b));
}

TEST(ArbitraryBool, Only) {
  auto arb = ac::make_arbitrary<bool>();
  arb.only_if([] (bool b) { return b; });

  ac::value<std::tuple<bool>> b;
  std::clog << std::boolalpha;
  for (int i = 0; i < limit; ++i) {
    arb(b);
    ASSERT_TRUE(std::get<0>(b.cref()));
    if (i > 0) std::clog << ", ";
    std::clog << b;
  }
  std::clog << std::endl;
}

TEST(ArbitraryBool, AtMost) {
  auto arb = ac::make_arbitrary<bool>();
  arb.only_if([] (bool b) { return b; });
  arb.at_most(limit);

  ac::value<std::tuple<bool>> b;
  std::clog << std::boolalpha;
  bool first = true;
  while (arb(b)) {
    if (first) {
      first = false;
    } else {
      std::clog << ", ";
    }
    ASSERT_TRUE(std::get<0>(b.cref()));
    std::clog << b;
  }
  std::clog << std::endl;

  ASSERT_FALSE(arb(b));
}

TEST(ArbitraryBool, Combinator) {
  auto arb =
    ac::at_most(limit,
        ac::only_if([] (bool b) { return !b; },
          ac::make_arbitrary<bool>()));

  ac::value<std::tuple<bool>> b;
  while (arb(b)) {
    ASSERT_FALSE(std::get<0>(b.cref()));
  }

  ASSERT_FALSE(arb(b));
}

