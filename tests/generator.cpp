#include <autocheck/generator.hpp>
#include <autocheck/sequence.hpp>

#include <gtest/gtest.h>

#include <iostream>

namespace ac = autocheck;

static const size_t limit = 10;

template <typename T>
void generate() {
  auto gen = ac::generator<T>();
  std::clog << "sizeof(gen) = " << sizeof(gen) << std::endl;

  std::clog << std::boolalpha;
  for (int i = 0; i < limit; ++i) {
    if (i > 0) std::clog << ", ";
    using namespace autocheck;
    std::clog << gen(i);
  }
  std::clog << '\n';
}

#define TEST_GENERATOR(name, type)\
  TEST(Generator, name) {\
    generate<type>();\
  }

TEST_GENERATOR(Bool, bool);

TEST_GENERATOR(Char,         char);
TEST_GENERATOR(SignedChar,   signed char);
TEST_GENERATOR(UnsignedChar, unsigned char);

TEST_GENERATOR(Short,         short);
TEST_GENERATOR(SignedShort,   signed short);
TEST_GENERATOR(UnsignedShort, unsigned short);

TEST_GENERATOR(Int,         int);
TEST_GENERATOR(SignedInt,   signed int);
TEST_GENERATOR(UnsignedInt, unsigned int);

TEST_GENERATOR(Long,         long);
TEST_GENERATOR(SignedLong,   signed long);
TEST_GENERATOR(UnsignedLong, unsigned long);

TEST_GENERATOR(Float,  float);
TEST_GENERATOR(Double, double);

TEST_GENERATOR(String, std::string);

TEST_GENERATOR(VectorOfInt, std::vector<int>);

