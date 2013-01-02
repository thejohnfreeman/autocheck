#ifndef AUTOCHECK_GENERATOR_HPP
#define AUTOCHECK_GENERATOR_HPP

#include <random>
#include <vector>
#include <iterator>

#include "is_one_of.hpp"
#include "predicate.hpp"
#include "generator_combinators.hpp"
#include "apply.hpp"

namespace autocheck {

  /* Reusable static standard random number generator. */
  static std::mt19937& rng() {
    static std::random_device rd;
    static std::mt19937 rng(rd());
    return rng;
  }

  template <typename T, typename... Gens, int... Is>
  T generate(std::tuple<Gens...>& gens, size_t size,
      const range<0, Is...>&)
  {
    return T(std::get<Is>(gens)(size)...);
  }

  template <typename T, typename... Gens>
  T generate(std::tuple<Gens...>& gens, size_t size) {
    return generate<T>(gens, size, range<sizeof...(Gens)>());
  }

  /* Generators produce an infinite sequence. */

  template <typename T, typename Enable = void>
  class generator;

  template <>
  class generator<bool> {
    public:
      typedef bool result_type;

      result_type operator() (size_t = 0) {
        static std::bernoulli_distribution dist(0.5);
        return dist(rng());
      }
  };

  template <typename UnsignedIntegral>
  class generator<
    UnsignedIntegral,
    typename std::enable_if<
      is_one_of<UnsignedIntegral, unsigned short, unsigned int,
        unsigned long, unsigned long long>::value
    >::type
  >
  {
    public:
      typedef UnsignedIntegral result_type;

      result_type operator() (size_t size) {
        /* Distribution is non-static. */
        std::uniform_int_distribution<UnsignedIntegral> dist(0, size);
        auto rv = dist(rng());
        return rv;
      }
  };

  template <typename SignedIntegral>
  class generator<
    SignedIntegral,
    typename std::enable_if<
      is_one_of<SignedIntegral, short, int, long, long long>::value
    >::type
  >
  {
    public:
      typedef SignedIntegral result_type;

      result_type operator() (size_t size) {
        /* Distribution is non-static. */
        std::uniform_int_distribution<SignedIntegral> dist(-size, size);
        auto rv = dist(rng());
        return rv;
      }
  };

  /* TODO: Generic sequence generator. */
  template <typename T, typename Gen = generator<T>>
  class list_generator {
    private:
      Gen eltgen;

    public:
      list_generator(const Gen& eltgen = Gen()) :
        eltgen(eltgen) {}

      typedef std::vector<T> result_type;

      result_type operator() (size_t size) {
        result_type rv;
        std::generate_n(std::back_insert_iterator<result_type>(rv), size,
            fix(size, eltgen));
        return rv;
      }
  };

  template <typename T, typename Gen = generator<T>>
  list_generator<T, Gen> list_of(const Gen& gen = Gen()) {
    return list_generator<T, Gen>(gen);
  }

  template <typename T>
  class generator<std::vector<T>> : public list_generator<T> {};

  /* TODO: Generic type generator (by construction). */
  template <typename T, typename... Gens>
  class cons_generator {
    private:
      std::tuple<Gens...> gens;

    public:
      cons_generator() :
        gens(Gens()...) {}

      cons_generator(const Gens&... gens) :
        gens(gens...) {}

      typedef T result_type;

      result_type operator() (size_t size) {
        return generate<result_type>(gens, (size > 0) ? (size - 1) : size);
      }
  };

  template <typename T, typename... Gens>
  cons_generator<T, Gens...> cons(const Gens&... gens) {
    return cons_generator<T, Gens...>(gens...);
  }

  template <typename T, typename... Args>
  cons_generator<T, generator<Args>...> cons() {
    return cons_generator<T, generator<Args>...>();
  }

}

#endif

