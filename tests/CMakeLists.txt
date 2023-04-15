cupcake_find_package(GTest 1.13.0)

set(tests
  apply
  arbitrary
  check
  generator_combinators
  generator
  is_one_of
  largest
  value
)

foreach(test ${tests})
  cupcake_add_test_executable(${test} ${test}.cpp)
  target_link_libraries(${this}
    autocheck::libautocheck
    GTest::gtest_main
  )
endforeach()
