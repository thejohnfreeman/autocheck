#ifndef AUTOCHECK_ARBITRARY_HPP
#define AUTOCHECK_ARBITRARY_HPP

#include <cassert>
#include <random>

#include "is_one_of.hpp"

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

      template <typename Derived>
      friend Derived& at_most(size_t, Derived&);
  };

  template <typename Derived>
  inline Derived& at_most(size_t limit, Derived& self) {
    self.is_limited = true;
    self.remaining  = limit;
    return self;
  }

  /* `arbitrary` is a class providing a set of standard models for the
   * Arbitrary concept. */

  template <typename T, typename Enable = void>
  class arbitrary;

  template <>
  class arbitrary<bool> : public limitable {
    public:
      bool operator() () {
        static std::random_device rd;
        static std::mt19937 rng(rd());
        static std::bernoulli_distribution dist(0.5);
        consume();
        return dist(rng);
      }
  };

}

#endif

