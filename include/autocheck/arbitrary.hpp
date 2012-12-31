#ifndef AUTOCHECK_ARBITRARY_HPP
#define AUTOCHECK_ARBITRARY_HPP

#include <cassert>
#include <random>
#include <cstdio>

#include "is_one_of.hpp"
#include "value.hpp"
#include "predicate.hpp"

namespace autocheck {

  /* Reusable mixin to support generation limits. */

  class limitable {
    private:
      bool   is_limited;
      size_t remaining;

    protected:
      limitable(size_t limit = 0)
        : is_limited(limit != 0), remaining(limit) {}

      void consume() {
        assert(remaining > 0);
        --remaining;
      }

    public:
      bool exhausted() const { return is_limited && !remaining; }

      void at_most(size_t limit) {
        is_limited = true;
        remaining  = limit;
      }
  };

  /* Sets a limit on a generator derived from `limitable`. */

  template <typename Derived>
  inline Derived&& at_most(size_t limit, Derived&& self) {
    self.at_most(limit);
    return std::forward<Derived>(self);
  }

  /* Decorator for filtering inputs. */

  template <typename Gen>
  class filtered_generator {
    public:
      typedef typename Gen::result_type result_type;

    private:
      typedef typename predicate<result_type>::type filter_t;
      typedef std::vector<filter_t>                 filters_t;

      Gen                base;
      value<result_type> candidate;
      filters_t          filters;

      bool accepted() const {
        return std::all_of(filters.begin(), filters.end(),
            [&] (const filter_t& f) { return f(candidate); });
      }

      void maybe_advance1() {
        assert(!candidate.empty());
        if (!accepted()) advance1();
      }

      void advance1() {
        while (!base.exhausted()) {
          candidate = base();
          if (accepted()) break;
        }
      }

    public:
      filtered_generator(const Gen& base)
        : base(base)
      {
        /* We must advance before the first call to `exhausted` in case no
         * generated values meet the filters. */
        advance1();
      }

      result_type operator() () {
        result_type rv(candidate);
        advance1();
        return rv;
      }

      bool exhausted() const { return base.exhausted(); }

      void only(const filter_t& f) {
        filters.push_back(f);
        maybe_advance1();
      }
  };

  /* Decorates an undecorated generator with a filter. */

  template <typename Gen>
  filtered_generator<Gen> only(
      const typename predicate<typename Gen::result_type>::type& f,
      const Gen& base)
  {
    filtered_generator<Gen> gen(base);
    gen.only(f);
    return std::move(gen);
  }

  /* Adds another filter to an already-decorated generator. */

  /* `arbitrary` is a class providing a set of standard models for the
   * Arbitrary concept. */

  template <typename T, typename Enable = void>
  class arbitrary;

  template <>
  class arbitrary<bool> : public limitable {
    public:
      typedef bool result_type;

      result_type operator() () {
        static std::random_device rd;
        static std::mt19937 rng(rd());
        static std::bernoulli_distribution dist(0.5);
        consume();
        return dist(rng);
      }
  };

}

#endif

