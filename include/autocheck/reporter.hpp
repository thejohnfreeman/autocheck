#ifndef AUTOCHECK_REPORTER_HPP
#define AUTOCHECK_REPORTER_HPP

#include <iostream>
#include <sstream>

namespace autocheck {

  class reporter {
    public:
      virtual void success(size_t tests, size_t max_tests) const = 0;
      virtual void failure(size_t tests, const char* reason) const = 0;
  };

  void report_success(std::ostream& out, size_t tests, size_t max_tests) {
    if (tests < max_tests) {
      out << "Arguments exhausted after " << tests << " tests."
        << std::endl;
    } else {
      assert(tests == max_tests);
      out << "OK, passed " << tests << " tests." << std::endl;
    }
  }

  void report_failure(std::ostream& out, size_t tests, const char* reason) {
    out << "Falsifiable, after " << tests << " tests:" << std::endl
      << reason << std::endl;
  }

  class ostream_reporter : public reporter {
    private:
      std::ostream& out;

    public:
      ostream_reporter(std::ostream& out = std::cout) : out(out) {}

      virtual void success(size_t tests, size_t max_tests) const {
        report_success(out, tests, max_tests);
      }

      virtual void failure(size_t tests, const char* reason) const {
        report_failure(out, tests, reason);
      }
  };

#ifdef ASSERT_TRUE

  class gtest_reporter : public reporter {
    public:
      virtual void success(size_t tests, size_t max_tests) const {
        report_success(std::clog, tests, max_tests);
        ASSERT_TRUE(true);
      }
      virtual void failure(size_t tests, const char* reason) const {
        std::ostringstream out;
        report_failure(out, tests, reason);
        static const bool AUTOCHECK_SUCCESS = false;
        ASSERT_TRUE(AUTOCHECK_SUCCESS) << out.str();
      }
  };

#endif // ASSERT_TRUE

}

#endif

