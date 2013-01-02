#ifndef AUTOCHECK_GENERATOR_HPP
#define AUTOCHECK_GENERATOR_HPP

#include <random>
#include <vector>
#include <iterator>
#include <limits>

#include "is_one_of.hpp"
#include "function.hpp"
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

  namespace detail {

    static const char alnums[]  = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz"
      "0123456789";
    /* Subtract 1 for NUL terminator. */
    static const size_t nalnums = sizeof(alnums) - 1;
    static const size_t nprint  = '~' - ' ' + 1;

  }

  enum CharCategory {
    ccAlphaNumeric,
    ccPrintable,
    ccAny
  };

  template <typename CharType, CharCategory Category = ccAny>
  class char_generator {
    public:
      typedef CharType result_type;

      result_type operator() (size_t size) {
        if (Category == ccAlphaNumeric || size < detail::nalnums) {
          size = detail::nalnums - 1;
        } else if (Category == ccPrintable || size < detail::nprint) {
          size = detail::nprint - 1;
        } else {
          size = std::numeric_limits<CharType>::max();
        }
        /* Distribution is non-static. */
        std::uniform_int_distribution<int> dist(0, size);
        auto i = dist(rng());
        auto rv =
          (size < detail::nalnums) ? detail::alnums[i] :
          ((size < detail::nprint) ? ' ' + i :
           i);
        return rv;
      }
  };

  /* WARNING: wchar_t, char16_t, char32_t, and family are typedefs. They
   * cannot be distinguished from regular (un)signed integrals, meaning we
   * cannot provide a general `generator` for them with the special char
   * consideration of size. If you want that consideration, use
   * char_generator specifically. */

  template <typename CharType>
  class generator<
    CharType,
    typename std::enable_if<
      is_one_of<CharType, unsigned char, char, signed char>::value
    >::type
  > : public char_generator<CharType> {};

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

  template <typename CharGen = generator<char>>
  class string_generator {
    private:
      CharGen chargen;

    public:
      string_generator(const CharGen& chargen = CharGen()) :
        chargen(chargen) {}

      typedef std::basic_string<typename CharGen::result_type> result_type;

      result_type operator() (size_t size) {
        result_type rv;
        rv.reserve(size);
        std::generate_n(std::back_insert_iterator<result_type>(rv), size,
            /* Scale characters faster than string size. */
            fix(size * 3, chargen));
        return rv;
      }
  };

  template <
    CharCategory Category = ccAny,
    typename CharType     = char
  >
  string_generator<char_generator<CharType, Category>>
  make_string_generator() {
    return string_generator<char_generator<CharType, Category>>();
  }

  template <typename CharGen>
  string_generator<CharGen>
  make_string_generator(const CharGen& chargen) {
    return string_generator<CharGen>();
  }

  template <typename CharType>
  class generator<std::basic_string<CharType>> :
    public string_generator<generator<CharType>> {};

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
        rv.reserve(size);
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

  /* Ordered list combinator. */

  template <typename T, typename Gen = generator<T>>
  detail::mapped_generator<std::vector<T>, list_generator<T, Gen>>
  ordered_list(const Gen& gen = Gen()) {
    /* This can be optimized if we care to. */
    return map<std::vector<T>>(
        [] (std::vector<T>&& a, size_t) {
          std::sort(a.begin(), a.end());
          return std::move(a);
        },
        list_of<T>(gen));
  }

  /* Generic type generator (by construction). */

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

