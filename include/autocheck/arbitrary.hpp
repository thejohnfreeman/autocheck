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
    public:
      typedef std::tuple<typename Gens::result_type...> result_type;

    private:
      typedef typename predicate<typename Gens::result_type...>::type
        premise_t;

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
      result_type generate(size_t size,
          const range<0, Is...>& = range<sizeof...(Gens)>())
      {
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
          out = generate(size());
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

  template <typename... Gens>
  arbitrary<Gens...> make_arbitrary(const Gens&... gens) {
    return arbitrary<Gens...>(gens...);
  }

}

#endif

