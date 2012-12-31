#include <iostream>
#include <gtest/gtest.h>
#include <autocheck/generator.hpp>

namespace ac = autocheck;

static const size_t limit = 10;

TEST(Generator, Sizes) {
  std::clog << "sizeof(generator<bool>) = "
    << sizeof(ac::generator<bool>) << std::endl;
}

TEST(GeneratorBool, Generating) {
  auto gen = ac::generator<bool>();

  std::clog << std::boolalpha;
  for (int i = 0; i < limit; ++i) {
    std::clog << gen() << ", ";
  }
  std::clog << std::endl;
}

TEST(Generator, Resize) {
  auto gen = ac::resize([] (size_t size) { return size * 2; },
      ac::generator<bool>());

  for (int i = 0; i < limit; ++i) {
    gen();
  }
}

TEST(Generator, SuchThat) {
  auto gen = ac::such_that([] (bool b) { return b; },
      ac::generator<bool>());

  for (int i = 0; i < limit; ++i) {
    ASSERT_TRUE(gen());
  }
}

