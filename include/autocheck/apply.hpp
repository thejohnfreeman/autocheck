#ifndef AUTOCHECK_APPLY_HPP
#define AUTOCHECK_APPLY_HPP

namespace autocheck {

  template <int N, int... Is>
  struct range : range<N - 1, N - 1, Is...> {};

  template <int... Is>
  struct range<0, Is...> {};

  template <typename F, typename Arg>
  typename std::result_of<F(Arg)>::type
  apply(F f, const Arg& arg) {
    return f(arg);
  }

  template <typename F, typename... Args>
  typename std::result_of<F(Args...)>::type
  apply(F f, const std::tuple<Args...>& args) {
    return subapply(f, args, range<sizeof...(Args)>());
  }

  template <typename F, typename... Args, int... Is>
  typename std::result_of<F(Args...)>::type
  subapply(F f, const std::tuple<Args...>& args, const range<0, Is...>&) {
    return f(std::get<Is>(args)...);
  }

}

#endif

