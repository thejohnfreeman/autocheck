#ifndef AUTOCHECK_CHECK_HPP
#define AUTOCHECK_CHECK_HPP

#include "predicate.hpp"
#include "arbitrary.hpp"
#include "reporter.hpp"

namespace autocheck {

  template <
    typename... Args,
    typename Generator = arbitrary<std::tuple<Args...>>
  >
  /* Cannot be deduced... even if Args specified. */
  //void check(size_t max_tests, const std::function<bool (Args...)>& pred,
  void check(size_t max_tests, const typename predicate<Args...>::type& pred,
      Generator gen = Generator(),
      const reporter& report = ostream_reporter())
  {
    assert(max_tests > 0);

    size_t tests = 0;

    typedef std::tuple<Args...> args_t;
    value<args_t> args;
    while (gen(args) && (++tests != max_tests)) {
      if (!apply(pred, args.cref())) {
        report.failure(tests, "<to be replaced with string of args>");
        return;
      }
    }

    report.success(tests, max_tests);
  }

}

#endif

