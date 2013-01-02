#ifndef AUTOCHECK_GENERATOR_COMBINATORS_HPP
#define AUTOCHECK_GENERATOR_COMBINATORS_HPP

namespace autocheck {

  namespace detail {

    /* Wrappers used by combinators. */

    template <typename T, typename Gen>
    class mapped_generator {
      public:
        typedef T
          result_type;
        typedef std::function<T (typename Gen::result_type&&, size_t)>
          func_t;

      private:
        func_t func;
        Gen    gen;

      public:
        mapped_generator(const func_t& func, const Gen& gen) :
          func(func), gen(gen) {}

        result_type operator() (size_t size = 0) {
          return func(gen(size), size);
        }
    };

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

    template <typename Gen>
    class fixed_size_generator {
      private:
        size_t size;
        Gen    gen;

      public:
        typedef typename Gen::result_type result_type;

        fixed_size_generator(size_t size, const Gen& gen) :
          size(size), gen(gen) {}

        result_type operator() () {
          return gen(size);
        }
    };

  }

  /* Generator combinators. */

  template <typename T, typename Gen>
  detail::mapped_generator<T, Gen> map(
      const typename detail::mapped_generator<T, Gen>::func_t& func,
      const Gen& gen)
  {
    return detail::mapped_generator<T, Gen>(func, gen);
  }

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

  template <typename Gen>
  detail::fixed_size_generator<Gen> fix(size_t size, const Gen& gen) {
    return detail::fixed_size_generator<Gen>(size, gen);
  }

}

#endif

