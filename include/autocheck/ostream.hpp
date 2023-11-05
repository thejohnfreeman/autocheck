#ifndef AUTOCHECK_OSTREAM_HPP
#define AUTOCHECK_OSTREAM_HPP

#include <ostream>
#include <tuple>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>

/* Forward declarations of output stream operators. */

namespace autocheck {

  template <typename... Ts>
  std::ostream& operator<< (std::ostream& out, const std::tuple<Ts...>& tup);

  template <typename T>
  std::ostream& operator<< (std::ostream& out, const std::vector<T>& seq);

  template <typename T>
  std::ostream& operator<< (std::ostream& out, const std::set<T>& seq);

  template <typename K, typename V>
  std::ostream& operator<< (std::ostream& out, const std::map<K, V>& seq);

  template <typename K, typename V>
  std::ostream& operator<< (std::ostream& out, const std::unordered_map<K, V>& seq);

}

#endif

