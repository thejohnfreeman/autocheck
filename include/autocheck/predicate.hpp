#ifndef AUTOCHECK_PREDICATE_HPP
#define AUTOCHECK_PREDICATE_HPP

#include <functional>

namespace autocheck {

  template <typename... Args>
  struct predicate {
    typedef std::function<bool (Args...)> type;
  };

}

#endif

