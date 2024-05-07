#include <algorithm>

#include <autocheck/autocheck.hpp>
namespace ac = autocheck;

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

struct prop_insert_sorted_t {
  template <typename T>
  bool operator() (const T& x, std::vector<T>& xs) {
    insert_sorted(x, xs);
    return std::is_sorted(xs.begin(), xs.end());
  }
};

template <typename T>
struct ordered_list_gen {
  ac::generator<std::vector<T>> source;

  typedef std::vector<T> result_type;

  std::vector<T> operator() (size_t size) {
    result_type xs(source(size));
    std::sort(xs.begin(), xs.end());
    return std::move(xs);
  }
};

int main() {
  ac::check<int, std::vector<int>>(prop_insert_sorted_t(),
      100, ac::make_arbitrary<int, std::vector<int>>());

  auto arb = ac::make_arbitrary<int, std::vector<int>>();
  arb.discard_if(
      [] (int, const std::vector<int>& xs) {
        return !std::is_sorted(xs.begin(), xs.end());
      });

  ac::check<int, std::vector<int>>(prop_insert_sorted_t(),
      100, ac::copy(arb));

  arb.discard_at_most(2000);

  ac::check<int, std::vector<int>>(prop_insert_sorted_t(),
      100, ac::copy(arb));

  arb.prep(
      [] (int, std::vector<int>& xs) {
        std::sort(xs.begin(), xs.end());
      });

  ac::check<int, std::vector<int>>(prop_insert_sorted_t(),
      100, ac::copy(arb));

  ac::check<int, std::vector<int>>(prop_insert_sorted_t(), 100,
      ac::make_arbitrary(ac::generator<int>(), ordered_list_gen<int>()));

  ac::check<int, std::vector<int>>(prop_insert_sorted_t(), 100,
      ac::make_arbitrary(ac::generator<int>(), ac::ordered_list<int>()));

  ac::classifier<int, std::vector<int>> cls;
  cls.trivial(
      [] (int, const std::vector<int>& xs) {
        return xs.empty();
      });

  ac::ostream_reporter rep;
  ac::check<int, std::vector<int>>(prop_insert_sorted_t(),
      100, ac::copy(arb), ac::copy(rep), ac::copy(cls));

  cls.collect([] (int x, const std::vector<int>& xs) { return xs.size(); });

  ac::check<int, std::vector<int>>(prop_insert_sorted_t(),
      100, ac::copy(arb), ac::copy(rep), ac::copy(cls));

  cls.classify([] (int x, const std::vector<int>& xs) { return xs.empty() || (x < xs.front()); }, "at-head");
  cls.classify([] (int x, const std::vector<int>& xs) { return xs.empty() || (xs.back() < x); }, "at-tail");

  ac::check<int, std::vector<int>>(prop_insert_sorted_t(),
      100, ac::copy(arb), ac::copy(rep), ac::copy(cls));

  return 0;
}

