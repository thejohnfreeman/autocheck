future_get_names_with_file_suffix(tests ".cpp")
foreach(test ${tests})
  future_add_test_executable(${test} EXCLUDE_FROM_ALL ${test}.cpp)
  target_link_libraries(${test}
    PRIVATE autocheck::autocheck GTest::gtest_main GTest::gtest
  )
endforeach()
