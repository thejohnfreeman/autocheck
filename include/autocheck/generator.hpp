#ifndef AUTOCHECK_GENERATOR_HPP
#define AUTOCHECK_GENERATOR_HPP

#include <random>

#include "predicate.hpp"

namespace autocheck {

  /* Generators produce an infinite sequence. */

  template <typename T, typename Enable = void>
  class generator;

  template <>
  class generator<bool> {
    public:
      typedef bool result_type;

      result_type operator() (size_t = 0) {
        static std::random_device rd;
        static std::mt19937 rng(rd());
        static std::bernoulli_distribution dist(0.5);
        return dist(rng);
      }
  };

  /* Type of functions that adjust size of generated value. */
  typedef std::function<size_t (size_t)> resizer_t;

  namespace detail {

    /* Wrappers used by combinators. */

    template <typename Gen>
    class filtered_generator {
      public:
        typedef typename Gen::result_type             result_type;
        typedef typename predicate<result_type>::type pred_t;

      private:
        pred_t pred;
        Gen    gen;

      public:
        filtered_generator(const pred_t& pred, const Gen& gen) :
          pred(pred), gen(gen) {}

        result_type operator() (size_t size = 0) {
          while (true) {
            result_type rv(gen(size));
            if (pred(rv)) return rv;
          }
        }
    };

    template <typename Gen>
    class resized_generator {
      public:
        typedef typename Gen::result_type result_type;

      private:
        resizer_t resizer;
        Gen       gen;

      public:
        resized_generator(const resizer_t& resizer, const Gen& gen) :
          resizer(resizer), gen(gen) {}

        result_type operator() (size_t size = 0) {
          return gen(resizer(size));
        }
    };

  }

  /* Generator combinators. */

  template <typename Gen>
  detail::filtered_generator<Gen> such_that(
      const typename detail::filtered_generator<Gen>::pred_t& pred,
      const Gen& gen)
  {
    return detail::filtered_generator<Gen>(pred, gen);
  }

  template <typename Gen>
  detail::resized_generator<Gen> resize(const resizer_t& resizer,
      const Gen& gen)
  {
    return detail::resized_generator<Gen>(resizer, gen);
  }

}

#endif

