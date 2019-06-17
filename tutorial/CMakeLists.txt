future_get_names_with_file_suffix(tutorials ".cpp")
foreach(tutorial ${tutorials})
  add_executable(${tutorial} EXCLUDE_FROM_ALL ${tutorial}.cpp)
  # We just need to test that we can compile the tutorials.
  add_test(
    NAME ${tutorial}
    COMMAND
      ${CMAKE_COMMAND}
      --build ${CMAKE_BINARY_DIR}
      --target ${tutorial}
      --config $<CONFIG>
    )
endforeach()

target_link_libraries(demonstrate_failure
  PRIVATE autocheck::autocheck GTest::gtest_main
)
target_link_libraries(factorial
  PRIVATE autocheck::autocheck Catch2::Catch2
)
target_link_libraries(insert_sorted PRIVATE autocheck::autocheck)
target_link_libraries(reverse PRIVATE autocheck::autocheck)
