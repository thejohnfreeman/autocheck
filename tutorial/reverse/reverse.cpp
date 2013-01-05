#include <algorithm>

#include <autocheck/autocheck.hpp>
namespace ac = autocheck;

template <typename Container>
bool prop_reverse_f(const Container& xs) {
  Container ys(xs);
  std::reverse(ys.begin(), ys.end());
  std::reverse(ys.begin(), ys.end());
  return xs == ys;
}

struct prop_reverse_t {
  template <typename Container>
  bool operator() (const Container& xs) const {
    return prop_reverse_f(xs);
  }
};

struct bad_prop_reverse_t {
  template <typename Container>
  bool operator() (const Container& xs) const {
    Container ys(xs);
    std::reverse(ys.begin(), ys.end());
    return xs == ys;
  }
};

int main() {
  //ac::check<std::vector<int>>(prop_reverse_t());
  ac::check<std::vector<int>>(prop_reverse_t(), 100, ac::make_arbitrary<std::vector<int>>());

  ac::check<std::vector<int>>(bad_prop_reverse_t(), 100, ac::make_arbitrary<std::vector<int>>());

  return 0;
}

