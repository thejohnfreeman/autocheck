#ifndef AUTOCHECK_ARBITRARY_HPP
#define AUTOCHECK_ARBITRARY_HPP

#include <cassert>
#include <cstdio>

#include "is_one_of.hpp"
#include "value.hpp"
#include "apply.hpp"

namespace autocheck {

  /* Generic identity function. */
  struct id {
    template <typename T>
    T&& operator() (T&& t) { return std::forward<T>(t); }
  };

  /* Model of our Arbitrary concept, which differs slightly from the one in
   * QuickCheck. */

  template <typename... Arbs>
  class arbitrary {
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
      arbitrary() : arbitrary(Arbs()...) {}

      arbitrary(const Arbs&... arbs) :
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

      arbitrary& at_most(size_t lmt) {
        assert(lmt > 0);
        is_limited = true;
        count      = 0;
        limit      = lmt + 1;
        return *this;
      }

      arbitrary& only(const filter_t& f) {
        filters.push_back(f);
        return *this;
      }

      arbitrary& resize(const resizer_t& f) {
        resizer = f;
        return *this;
      }
  };

  /* Combinators. */

  template <typename... Arbs>
  arbitrary<Arbs...>&& at_most(size_t limit, arbitrary<Arbs...>&& self) {
    self.at_most(limit);
    return std::forward<arbitrary<Arbs...>>(self);
  }

  template <typename... Arbs>
  arbitrary<Arbs...>&& only(const typename predicate<Arbs...>::type& f,
      arbitrary<Arbs...>&& self)
  {
    self.only(f);
    return std::forward<arbitrary<Arbs...>>(self);
  }

  template <typename... Arbs>
  arbitrary<Arbs...>&& resize(const resizer_t& f, arbitrary<Arbs...>&& self) {
    self.resize(f);
    return std::forward<arbitrary<Arbs...>>(self);
  }

  template <typename... Ts>
  arbitrary<detail::generator<Ts>...> generator() {
    return arbitrary<detail::generator<Ts>...>();
  }

}

#endif

