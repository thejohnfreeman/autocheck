#include <iostream>
#include <gtest/gtest.h>
#include <autocheck/generator.hpp>

namespace ac = autocheck;

static const size_t limit = 10;

TEST(GeneratorBool, Generating) {
  auto gen = ac::generator<bool>();
  std::clog << "sizeof(gen) = " << sizeof(gen) << std::endl;

  std::clog << std::boolalpha;
  for (int i = 0; i < limit; ++i) {
    if (i > 0) std::clog << ", ";
    std::clog << gen();
  }
  std::clog << std::endl;
}

TEST(Generator, Map) {
  auto gen = ac::map([] (bool, size_t n) { return n; },
      ac::generator<bool>());
  std::clog << "sizeof(gen) = " << sizeof(gen) << std::endl;

  const int n = 42;
  for (int i = 0; i < limit; ++i) {
    ASSERT_EQ(n, gen(n));
  }
}

TEST(Generator, SuchThat) {
  auto gen = ac::such_that([] (bool b) { return b; },
      ac::generator<bool>());
  std::clog << "sizeof(gen) = " << sizeof(gen) << std::endl;

  for (int i = 0; i < limit; ++i) {
    ASSERT_TRUE(gen());
  }
}

TEST(Generator, Resize) {
  auto gen = ac::resize([] (size_t size) { return size * 2; },
      ac::generator<bool>());
  std::clog << "sizeof(gen) = " << sizeof(gen) << std::endl;

  for (int i = 0; i < limit; ++i) {
    gen();
  }
}

TEST(Generator, Composition) {
  const int factor = 2;
  auto gen = ac::resize([] (size_t size) { return size * factor; },
      ac::map([] (bool, size_t n) { return n; },
        ac::generator<bool>()));
  std::clog << "sizeof(gen) = " << sizeof(gen) << std::endl;

  const int n = 42;
  for (int i = 0; i < limit; ++i) {
    ASSERT_EQ(n * factor, gen(n));
  }
}

