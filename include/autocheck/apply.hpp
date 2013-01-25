#ifndef AUTOCHECK_APPLY_HPP
#define AUTOCHECK_APPLY_HPP

namespace autocheck {

  template <int N, int... Is>
  struct range : range<N - 1, N - 1, Is...> {};

  template <int... Is>
  struct range<0, Is...> {};

  /* Const version. */

  template <typename F, typename... Args>
  /* Cannot put const in the parameter list here for some reason. Breaks in
   * GCC 4.7.2 and Clang 3.2. Works in Clang 3.3. Doesn't seem right, but
   * we'll do it this way for now. */
  typename std::result_of<F(Args&...)>::type
  apply(F f, const std::tuple<Args...>& args) {
    return subapply(f, args, range<sizeof...(Args)>());
  }

  template <typename F, typename... Args, int... Is>
  typename std::result_of<F(Args&...)>::type
  subapply(F f, const std::tuple<Args...>& args, const range<0, Is...>&) {
    return f(std::get<Is>(args)...);
  }

  /* Non-const version. */

  template <typename F, typename... Args>
  typename std::result_of<F(Args&...)>::type
  apply(F f, std::tuple<Args...>& args) {
    return subapply(f, args, range<sizeof...(Args)>());
  }

  template <typename F, typename... Args, int... Is>
  typename std::result_of<F(Args&...)>::type
  subapply(F f, std::tuple<Args...>& args, const range<0, Is...>&) {
    return f(std::get<Is>(args)...);
  }

}

#endif

