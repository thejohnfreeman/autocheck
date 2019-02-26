# file(GLOB TEST_SOURCE_PATHS "*.cpp" CONFIGURE_DEPENDS)
get_names_with_file_suffix(TESTS ".cpp")
foreach(TEST ${TESTS})
  add_executable(${TEST} EXCLUDE_FROM_ALL ${TEST}.cpp)
  target_link_libraries(${TEST}
    PRIVATE
      autocheck::autocheck
      gtest::gtest
  )
  add_test_from_target(${TEST})
endforeach(TEST ${TESTS})
