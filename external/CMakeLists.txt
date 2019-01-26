# Add googletest only if a dependent project has not already added it for us.
if(NOT TARGET gtest_main)
  add_subdirectory(googletest)
endif(NOT TARGET gtest_main)
