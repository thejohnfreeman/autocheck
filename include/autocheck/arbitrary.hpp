#ifndef AUTOCHECK_ARBITRARY_HPP
#define AUTOCHECK_ARBITRARY_HPP

#include <cassert>
#include <cstdio>

#include "generator.hpp"
#include "value.hpp"
#include "apply.hpp"

namespace autocheck {

  /* Generic identity function. */
  struct id {
    template <typename T>
    T&& operator() (T&& t) const { return std::forward<T>(t); }
  };

  struct always {
    template <typename T>
    bool operator() (const T& t) const { return true; }
  };

  /* Arbitrary produces a finite sequence, always in a tuple, ready for
   * application, and counts discards. */

  template <typename... Gens>
  class arbitrary {
    static_assert(sizeof...(Gens) > 0, "cannot create unit arbitrary");
    public:
      typedef std::tuple<typename Gens::result_type...> result_type;

      typedef typename predicate<typename Gens::result_type...>::type
        premise_t;

    private:
      std::tuple<Gens...> gens;
      bool                is_finite;
      size_t              count;
      size_t              num_discards;
      size_t              max_discards;
      premise_t           premise;
      resizer_t           resizer;

      /* Returns 0 on first call, and grows moderately. */
      size_t size() const {
        return resizer(count >> 1);
      }

      bool accepted(const result_type& candidate) const {
        return apply(premise, candidate);
      }

      template <int... Is>
      result_type generate(size_t size, const range<0, Is...>&) {
        return result_type(std::get<Is>(gens)(size)...);
      }

    public:
      template <typename... Args>
      arbitrary(const Args&... args) :
        gens(args...),
        is_finite(false), count(0), num_discards(0), max_discards(0),
        premise(always()), resizer(id())
      {}

      bool operator() (value<result_type>& out) {
        while (!is_finite || (num_discards < max_discards)) {
          out = generate(size(), range<sizeof...(Gens)>());
          if (accepted(out)) {
            return true;
          } else {
            ++num_discards;
          }
        }
        return false;
      }

      arbitrary& reset() {
        num_discards = 0;
        count        = 0;
      }

      arbitrary& at_most(size_t discards) {
        assert(discards > 0);
        is_finite    = true;
        max_discards = discards;
        return *this;
      }

      arbitrary& only_if(const premise_t& p) {
        premise = p;
        return *this;
      }

      arbitrary& resize(const resizer_t& r) {
        resizer = r;
        return *this;
      }
  };

  /* Combinators. */

  template <typename... Gens>
  arbitrary<Gens...>&& at_most(size_t discards, arbitrary<Gens...>&& arb) {
    arb.at_most(discards);
    return std::forward<arbitrary<Gens...>>(arb);
  }

  template <typename... Gens>
  arbitrary<Gens...>&& only_if(
      const typename arbitrary<Gens...>::premise_t& p,
      arbitrary<Gens...>&& arb)
  {
    arb.only_if(p);
    return std::forward<arbitrary<Gens...>>(arb);
  }

  template <typename... Gens>
  arbitrary<Gens...>&& resize(const resizer_t& r, arbitrary<Gens...>&& arb) {
    arb.resize(r);
    return std::forward<arbitrary<Gens...>>(arb);
  }

  /* Factories. */

  template <typename... Gens>
  arbitrary<Gens...> make_arbitrary(const Gens&... gens) {
    return arbitrary<Gens...>(gens...);
  }

  template <typename... Types>
  arbitrary<generator<Types...>> make_arbitrary() {
    return arbitrary<generator<Types...>>(generator<Types>()...);
  }

}

#endif

