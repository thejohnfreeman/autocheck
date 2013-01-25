#ifndef AUTOCHECK_APPLY_HPP
#define AUTOCHECK_APPLY_HPP

#include <type_traits>
#include <tuple>

namespace autocheck {

  /* To extend for more fixed-size containers, just overload result_of_apply,
   * std::tuple_size, and std::get. */

  template <typename T, typename U>
  struct forward_reference {
    typedef U type;
  };

  template <typename T, typename U>
  struct forward_reference<T&, U> {
    typedef U& type;
  };

  template <typename T, typename U>
  struct forward_reference<T&&, U> {
    typedef U&& type;
  };


  template <size_t N, size_t... Is>
  struct range : range<N - 1, N - 1, Is...> {};

  template <size_t... Is>
  struct range<0, Is...> {};


  template <typename T>
  struct bare_type {
    typedef typename std::remove_cv<
      typename std::remove_reference<T>::type
    >::type type;
  };


  template <typename T, size_t N, typename... Ts>
  struct array_tuple : array_tuple<T, N - 1, T, Ts...> {};

  template <typename T, typename... Ts>
  struct array_tuple<T, 0, Ts...> : std::tuple<Ts...> {};


  template <
    typename F,
    typename Tuple,
    typename Select = typename bare_type<Tuple>::type
  >
  struct result_of_apply;

  template <typename F, typename Tuple, typename... Args>
  struct result_of_apply<F, Tuple, std::tuple<Args...>> {
    typedef typename std::result_of<
      F(typename forward_reference<Tuple, Args>::type...)
    >::type type;
  };

  template <typename F, typename Tuple, typename T1, typename T2>
  struct result_of_apply<F, Tuple, std::pair<T1, T2>> {
    typedef typename std::result_of<
      F(typename forward_reference<Tuple, T1>::type,
        typename forward_reference<Tuple, T2>::type)
    >::type type;
  };

  template <typename F, typename Tuple, typename T, size_t N>
  struct result_of_apply<F, Tuple, std::array<T, N>> :
    result_of_apply<F, Tuple, array_tuple<T, N>> {};


  template <typename F, typename Tuple, size_t... Is>
  typename result_of_apply<F, Tuple>::type
  subapply(F f, Tuple&& args, const range<0, Is...>&) {
    return f(std::get<Is>(std::forward<Tuple>(args))...);
  }

  template <typename F, typename Tuple>
  typename result_of_apply<F, Tuple>::type
  apply(F f, Tuple&& args) {
    return subapply(f,
        std::forward<Tuple>(args),
        range<std::tuple_size<typename std::decay<Tuple>::type>::value>());
  }

}

#endif

