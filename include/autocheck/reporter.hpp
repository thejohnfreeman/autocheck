#ifndef AUTOCHECK_REPORTER_HPP
#define AUTOCHECK_REPORTER_HPP

#include <ostream>

namespace autocheck {

  class reporter {
    public:
      virtual void success(size_t tests, size_t max_tests) = 0;
      virtual void failure(size_t tests, const char* reason) = 0;
  };

  class ostream_reporter : public reporter {
    private:
      std::ostream& out;
    public:
      ostream_reporter(std::ostream& out) : out(out) {}

      virtual void success(size_t tests, size_t max_tests) {
        if (tests < max_tests) {
          out << "Arguments exhausted after " << tests << " tests."
            << std::endl;
        } else {
          assert(tests == max_tests);
          out << "OK, passed " << tests << " tests." << std::endl;
        }
      }

      virtual void failure(size_t tests, const char* reason) {
        out << "Falsifiable, after " << tests << " tests:" << std::endl
          << reason << std::endl;
      }
  };

}

#endif

