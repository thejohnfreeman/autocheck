set(demonstrate_failure_DEPENDS autocheck::autocheck gtest::gtest)
set(factorial_DEPENDS autocheck::autocheck catch2::catch2)
set(insert_sorted_DEPENDS autocheck::autocheck)
set(reverse_DEPENDS autocheck::autocheck)

# file(GLOB TUTORIAL_SOURCE_PATHS "*.cpp" CONFIGURE_DEPENDS)
file(GLOB TUTORIAL_SOURCE_PATHS "*.cpp")
foreach(TUTORIAL_SOURCE_PATH ${TUTORIAL_SOURCE_PATHS})
  get_filename_component(TUTORIAL_SOURCE_NAME ${TUTORIAL_SOURCE_PATH} NAME)
  string(REPLACE ".cpp" "" TARGET ${TUTORIAL_SOURCE_NAME})
  add_executable(${TARGET} EXCLUDE_FROM_ALL ${TUTORIAL_SOURCE_PATH})
  target_link_libraries(${TARGET} PRIVATE ${${TARGET}_DEPENDS})
  # We just need to test that we can compile the tutorials.
  add_test(
    NAME ${TARGET}
    COMMAND
      ${CMAKE_COMMAND}
        --build ${CMAKE_BINARY_DIR}
        --target ${TARGET}
        --config $<CONFIG>
    )
endforeach(TUTORIAL_SOURCE_PATH ${TUTORIAL_SOURCE_PATHS})
