# https://stackoverflow.com/a/10824578/618906
function(add_test_from_target TARGET)
  add_test(
    NAME ${TARGET}_build
    COMMAND
      ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${TARGET}
    )
  add_test(NAME ${TARGET} COMMAND ${TARGET})
  set_tests_properties(${TARGET} PROPERTIES DEPENDS ${TARGET}_build)
endfunction(add_test_from_target TARGET)

# file(GLOB TEST_SOURCE_PATHS "*.cpp" CONFIGURE_DEPENDS)
file(GLOB TEST_SOURCE_PATHS "*.cpp")
foreach(TEST_SOURCE_PATH ${TEST_SOURCE_PATHS})
  get_filename_component(TEST_SOURCE_NAME ${TEST_SOURCE_PATH} NAME)
  string(REPLACE ".cpp" "" TARGET ${TEST_SOURCE_NAME})
  add_executable(${TARGET} EXCLUDE_FROM_ALL ${TEST_SOURCE_PATH})
  target_link_libraries(${TARGET} PRIVATE autocheck::autocheck)
  target_link_libraries(${TARGET} PRIVATE GTest::Main)
  add_test_from_target(${TARGET})
endforeach(TEST_SOURCE_PATH ${TEST_SOURCE_PATHS})
