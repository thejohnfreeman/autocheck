#ifndef AUTOCHECK_ARBITRARY_HPP
#define AUTOCHECK_ARBITRARY_HPP

#include <cassert>
#include <cstdio>

#include "function.hpp"
#include "generator.hpp"
#include "value.hpp"
#include "apply.hpp"

namespace autocheck {

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

    public:
      arbitrary(const Gens&... gens) :
        gens(gens...),
        is_finite(false), count(0), num_discards(0), max_discards(0),
        premise(always()), resizer(id())
      {}

      bool operator() (value<result_type>& candidate) {
        while (!is_finite || (num_discards < max_discards)) {
          /* Size starts at 0 and grows moderately. */
          candidate = generate<result_type>(gens, resizer(count >> 1));
          if (apply(premise, candidate.cref())) {
            ++count;
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

  template <typename Arbitrary>
  Arbitrary&& at_most(size_t discards, Arbitrary&& arb) {
    arb.at_most(discards);
    return std::forward<Arbitrary>(arb);
  }

  template <typename Arbitrary>
  Arbitrary&& only_if(const typename Arbitrary::premise_t& p,
      Arbitrary&& arb)
  {
    arb.only_if(p);
    return std::forward<Arbitrary>(arb);
  }

  template <typename Arbitrary>
  Arbitrary&& resize(const resizer_t& r, Arbitrary&& arb) {
    arb.resize(r);
    return std::forward<Arbitrary>(arb);
  }

  /* Factories. */

  template <typename... Gens>
  arbitrary<Gens...> make_arbitrary(const Gens&... gens) {
    return arbitrary<Gens...>(gens...);
  }

  template <typename... Args>
  arbitrary<generator<Args...>> make_arbitrary() {
    return arbitrary<generator<Args...>>(generator<Args>()...);
  }

}

#endif

