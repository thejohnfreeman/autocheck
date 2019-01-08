#include <gtest/gtest.h>
#include <autocheck/value.hpp>

struct leaker {
  static int alive;
  leaker()              { ++alive; }
  leaker(int*)          { ++alive; }
  leaker(const leaker&) { ++alive; }
  ~leaker()             { --alive; }
};

int leaker::alive = 0;

namespace ac = autocheck;

struct Value : public ::testing::Test {
  Value() { leaker::alive = 0; }

  void TearDown() {
    ASSERT_EQ(0, leaker::alive);
  }
};

TEST_F(Value, Default) {
  ac::value<leaker> val;
}

TEST_F(Value, ConstructDefault) {
  ac::value<leaker> val;
  val.construct();
}

TEST_F(Value, ConstructUnary) {
  ac::value<leaker> val;
  val.construct(nullptr);
}

TEST_F(Value, Heap) {
  ac::value<leaker> val;
  val.ptr(new leaker());
}

TEST_F(Value, Assign) {
  ac::value<leaker> val;
  val = new leaker();
  val = leaker();
  val = leaker();
  val = new leaker();
}

TEST_F(Value, Read) {
  ac::value<int> val;
  val = 1;
  ASSERT_EQ(1, val);
  val = 2;
  ASSERT_EQ(2, val);
  val = new int(3);
  ASSERT_EQ(3, val);
  val = 4;
  ASSERT_EQ(4, val);
}

