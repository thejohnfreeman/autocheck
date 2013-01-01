#ifndef AUTOCHECK_TUPLE_HPP
#define AUTOCHECK_TUPLE_HPP

namespace autocheck {
  namespace detail {

    template <size_t I, typename... Ts>
    void print(std::ostream& out, const std::tuple<Ts...>& tup,
        const std::integral_constant<size_t, I>&
          = std::integral_constant<size_t, I>())
    {
      out << std::get<I>(tup);
      out << ", ";
    }

    template <typename... Ts>
    void print(std::ostream& out, const std::tuple<Ts...>& tup,
        const std::integral_constant<size_t, 0>&)
    {
      out << std::get<0>(tup);
    }

  }
}

namespace std {

  template <typename... Ts>
  std::ostream& operator<< (std::ostream& out, const std::tuple<Ts...>& tup) {
    out << "(";
    autocheck::detail::print(out, tup,
        std::integral_constant<size_t, sizeof...(Ts) - 1>());
    out << ")";
    return out;
  }

}

#endif

