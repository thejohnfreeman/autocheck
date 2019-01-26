# Add googletest only if a dependent project has not already added it for us.
find_package(GTest)
if(NOT GTEST_FOUND)
  add_subdirectory(googletest)
  add_library(GTest::Main ALIAS gtest_main)
  set(GTEST_FOUND 1)
endif(NOT GTEST_FOUND)
