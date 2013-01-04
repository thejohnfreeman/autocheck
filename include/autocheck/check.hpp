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
    typename Predicate,
    typename Arbitrary = arbitrary<std::tuple<Args...>>
    //, typename Classifier = classifier<Args...> // ICEs Clang
  >
  void check(size_t max_tests, Predicate pred,
      Arbitrary&& arb = Arbitrary(),
      const reporter& rep = ostream_reporter(),
      classifier<Args...>&& cls = classifier<Args...>())
      //Classifier&& cls = Classifier())
  {
    assert(max_tests > 0);

    size_t tests = 0;

    typedef std::tuple<Args...> args_t;
    value<args_t> args;
    while (arb(args) && (++tests != max_tests)) {
      /* Get what we need from `args` before we let the user modify them. */
      std::ostringstream reason;
      reason << std::boolalpha << args;
      //std::cout << "args = " << args << std::endl;

      cls.check(args.cref());
      if (!apply(pred, args.ref())) {
        rep.failure(tests, reason.str().c_str());
        return;
      }
    }

    rep.success(tests, max_tests, cls.trivial(), cls.distro());
  }

}

#endif

