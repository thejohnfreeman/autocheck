#ifndef AUTOCHECK_REPORTER_HPP
#define AUTOCHECK_REPORTER_HPP

#include <iostream>

namespace autocheck {

  class reporter {
    public:
      virtual void success(size_t tests, size_t max_tests) const = 0;
      virtual void failure(size_t tests, const char* reason) const = 0;
  };

  class ostream_reporter : public reporter {
    private:
      std::ostream& out;

    public:
      ostream_reporter(std::ostream& out = std::cout) : out(out) {}

      virtual void success(size_t tests, size_t max_tests) const {
        if (tests < max_tests) {
          out << "Arguments exhausted after " << tests << " tests."
            << std::endl;
        } else {
          assert(tests == max_tests);
          out << "OK, passed " << tests << " tests." << std::endl;
        }
      }

      virtual void failure(size_t tests, const char* reason) const {
        out << "Falsifiable, after " << tests << " tests:" << std::endl
          << reason << std::endl;
      }
  };

}

#endif

