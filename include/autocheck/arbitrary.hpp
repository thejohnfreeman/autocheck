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

    template <typename... Bases>
    class tuple_generator {
      private:
        std::tuple<Bases...> bases;

      public:
        typedef std::tuple<typename Bases::result_type...> result_type;

        tuple_generator() :
          bases(Bases()...) {}

        tuple_generator(const Bases&... bases) :
          bases(bases...) {}

        template <int... Is>
        result_type generate(size_t size, const range<0, Is...>&) {
          return result_type(std::get<Is>(bases)(size)...);
        }

        result_type operator() (size_t size) {
          return generate(size, range<sizeof...(Bases)>());
        }
    };

    /* Base generators for arbitrary. */
    template <typename T, typename Enable = void>
    class generator;

    template <typename... Args>
    class generator<std::tuple<Args...>> :
      public tuple_generator<generator<Args>...>
    {
      private:
        typedef tuple_generator<generator<Args>...> base_t;

      public:
        generator() : base_t() {}

        generator(const generator<Args>&... bases) :
          base_t(bases...) {}
    };

    template <>
    class generator<bool> {
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

  template <typename T, typename Base = detail::generator<T>>
  class arbitrary {
    public:
      typedef T result_type;
      static_assert(
          std::is_convertible<typename Base::result_type, result_type>::value,
          "incompatible Base generator");

    private:
      typedef typename predicate<result_type>::type filter_t;
      typedef std::vector<filter_t>                 filters_t;

      Base      base;
      bool      is_limited;
      size_t    count;
      size_t    limit;
      filters_t filters;
      resizer_t resizer;

      /* Returns 0 on first call, and grows moderately. */
      size_t size() const {
        return resizer(count >> 1);
      }

      bool accepted(const result_type& candidate) const {
        return std::all_of(filters.begin(), filters.end(),
            [&] (const filter_t& accepts) { return accepts(candidate); });
      }

    public:
      arbitrary(const Base& base = Base()) :
        base(base),
        is_limited(false), count(0), limit(0),
        filters(), resizer(id())
      {}

      bool operator() (value<result_type>& out) {
        while (!is_limited || (++count < limit)) {
          out = base(size());
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

  template <typename T, typename Base>
  arbitrary<T, Base>&& at_most(size_t limit, arbitrary<T, Base>&& self) {
    self.at_most(limit);
    return std::forward<arbitrary<T, Base>>(self);
  }

  template <typename T, typename Base>
  arbitrary<T, Base>&& only(const typename predicate<T>::type& f,
      arbitrary<T, Base>&& self)
  {
    self.only(f);
    return std::forward<arbitrary<T, Base>>(self);
  }

  template <typename T, typename Base>
  arbitrary<T, Base>&& resize(const resizer_t& f, arbitrary<T, Base>&& self) {
    self.resize(f);
    return std::forward<arbitrary<T, Base>>(self);
  }

}

#endif

