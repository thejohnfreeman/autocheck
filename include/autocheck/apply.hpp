#ifndef AUTOCHECK_APPLY_HPP
#define AUTOCHECK_APPLY_HPP

#include <type_traits>
#include <tuple>

namespace autocheck {

  template <int N, int... Is>
  struct range : range<N - 1, N - 1, Is...> {};

  template <int... Is>
  struct range<0, Is...> {};

  /* Const version. */

  template <typename F, typename Tuple>
  struct result_of_apply {};

  template <typename F, typename... Args>
  struct result_of_apply<F, std::tuple<Args...>> {
    typedef typename std::result_of<F(Args&...)>::type type;
  };

  template <typename F, typename Tuple>
  /* Cannot put const in the parameter list here for some reason. Breaks in
   * GCC 4.7.2 and Clang 3.2. Works in Clang 3.3. Doesn't seem right, but
   * we'll do it this way for now. */
  typename result_of_apply<F, typename std::decay<Tuple>::type>::type
  apply(F f, Tuple&& args) {
    return subapply(f,
        std::forward<Tuple>(args),
        range<std::tuple_size<typename std::decay<Tuple>::type>::value>());
  }

  template <typename F, typename Tuple, int... Is>
  typename result_of_apply<F, typename std::decay<Tuple>::type>::type
  subapply(F f, Tuple&& args, const range<0, Is...>&) {
    return f(std::get<Is>(std::forward<Tuple>(args))...);
  }

}

#endif

