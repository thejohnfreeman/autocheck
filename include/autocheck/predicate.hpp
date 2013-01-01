#ifndef AUTOCHECK_PREDICATE_HPP
#define AUTOCHECK_PREDICATE_HPP

#include <functional>

namespace autocheck {

  template <typename... Args>
  struct predicate {
    typedef std::function<bool (Args...)> type;
  };

  /* Type of functions that adjust size of generated value. */
  typedef std::function<size_t (size_t)> resizer_t;

}

#endif

