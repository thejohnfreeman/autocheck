#ifndef AUTOCHECK_ARBITRARY_HPP
#define AUTOCHECK_ARBITRARY_HPP

#include <cassert>
#include <random>
#include <cstdio>

#include "is_one_of.hpp"
#include "value.hpp"
#include "predicate.hpp"
#include "apply.hpp"

namespace autocheck {

  /* Type of functions that adjust size of generated value. */
  typedef std::function<size_t (size_t)> resizer_t;

  /* Generic identity function. */
  struct id {
    template <typename T>
    T&& operator() (T&& t) { return std::forward<T>(t); }
  };

  namespace detail {

    /* Base generators for generator. */
    template <typename T, typename Enable = void>
    class arbitrary;

    template <>
    class arbitrary<bool> {
      public:
        typedef bool result_type;

        result_type operator() (size_t) {
          static std::random_device rd;
          static std::mt19937 rng(rd());
          static std::bernoulli_distribution dist(0.5);
          return dist(rng);
        }
    };

  }

  /* Model of our Arbitrary concept, which differs slightly from the one in
   * QuickCheck. */

  template <typename... Arbs>
  class generator {
    public:
      typedef std::tuple<typename Arbs::result_type...> result_type;

    private:
      typedef typename predicate<typename Arbs::result_type...>::type
        filter_t;
      typedef std::vector<filter_t>
        filters_t;

      std::tuple<Arbs...> arbs;
      bool                is_limited;
      size_t              count;
      size_t              limit;
      filters_t           filters;
      resizer_t           resizer;

      /* Returns 0 on first call, and grows moderately. */
      size_t size() const {
        return resizer(count >> 1);
      }

      bool accepted(const result_type& candidate) const {
        return std::all_of(filters.begin(), filters.end(),
            [&] (const filter_t& accepts) {
              return apply(accepts, candidate); });
      }

      template <int... Is>
      result_type generate(size_t size,
          const range<0, Is...>& = range<sizeof...(Arbs)>())
      {
        return result_type(std::get<Is>(arbs)(size)...);
      }

    public:
      generator() : generator(Arbs()...) {}

      generator(const Arbs&... arbs) :
        arbs(arbs...),
        is_limited(false), count(0), limit(0),
        filters(), resizer(id())
      {}

      bool operator() (value<result_type>& out) {
        while (!is_limited || (++count < limit)) {
          out = generate(size());
          if (accepted(out)) return true;
        }
        return false;
      }

      generator& at_most(size_t lmt) {
        assert(lmt > 0);
        is_limited = true;
        count      = 0;
        limit      = lmt + 1;
        return *this;
      }

      generator& only(const filter_t& f) {
        filters.push_back(f);
        return *this;
      }

      generator& resize(const resizer_t& f) {
        resizer = f;
        return *this;
      }
  };

  /* Combinators. */

  template <typename... Arbs>
  generator<Arbs...>&& at_most(size_t limit, generator<Arbs...>&& self) {
    self.at_most(limit);
    return std::forward<generator<Arbs...>>(self);
  }

  template <typename... Arbs>
  generator<Arbs...>&& only(const typename predicate<Arbs...>::type& f,
      generator<Arbs...>&& self)
  {
    self.only(f);
    return std::forward<generator<Arbs...>>(self);
  }

  template <typename... Arbs>
  generator<Arbs...>&& resize(const resizer_t& f, generator<Arbs...>&& self) {
    self.resize(f);
    return std::forward<generator<Arbs...>>(self);
  }

  template <typename... Ts>
  generator<detail::arbitrary<Ts>...> arbitrary() {
    return generator<detail::arbitrary<Ts>...>();
  }

}

#endif

