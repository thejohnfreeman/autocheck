#ifndef AUTOCHECK_CHECK_HPP
#define AUTOCHECK_CHECK_HPP

#include <sstream>

#include "function.hpp"
#include "arbitrary.hpp"
#include "tuple.hpp"
#include "reporter.hpp"
#include "classifier.hpp"

namespace autocheck {

  template <
    typename... Args,
    typename Arbitrary = arbitrary<std::tuple<Args...>>
  >
  /* Cannot be deduced... even if Args specified. */
  //void check(size_t max_tests, const std::function<bool (Args...)>& pred,
  void check(size_t max_tests, const typename predicate<Args...>::type& pred,
      Arbitrary&& arb = make_arbitrary<Args...>(),
      reporter&& rep = ostream_reporter(),
      classifier<Args...>&& cls = classifier<Args...>())
  {
    assert(max_tests > 0);

    size_t tests = 0;

    typedef std::tuple<Args...> args_t;
    value<args_t> args;
    while (arb(args) && (++tests != max_tests)) {
      //std::cout << "args = " << args << std::endl;
      cls.check(args.cref());
      if (!apply(pred, args.cref())) {
        std::ostringstream reason;
        reason << std::boolalpha << args;
        rep.failure(tests, reason.str().c_str());
        return;
      }
    }

    rep.success(tests, max_tests, cls.trivial(), cls.distro());
  }

}

#endif

