# Need 3.1 for C++11.
# Need 3.12 for CONFIGURE_DEPENDS.
# https://cliutils.gitlab.io/modern-cmake/chapters/intro/newcmake.html
cmake_minimum_required(VERSION 3.1)
project(autocheck VERSION 1.0.0 LANGUAGES CXX)

add_subdirectory(external/googletest)

add_library(autocheck INTERFACE)
target_include_directories(autocheck
  INTERFACE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${autocheck_SOURCE_DIR}/include>
)
target_compile_features(autocheck INTERFACE cxx_lambdas)

enable_testing()

# file(GLOB TEST_SOURCE_PATHS "tests/*.cpp" CONFIGURE_DEPENDS)
file(GLOB TEST_SOURCE_PATHS "tests/*.cpp")
foreach(TEST_SOURCE_PATH ${TEST_SOURCE_PATHS})
  get_filename_component(TEST_SOURCE_NAME ${TEST_SOURCE_PATH} NAME)
  string(REPLACE ".cpp" "" TEST_NAME ${TEST_SOURCE_NAME})
  add_executable(${TEST_NAME} ${TEST_SOURCE_PATH})
  target_link_libraries(${TEST_NAME} PRIVATE autocheck)
  target_link_libraries(${TEST_NAME} PRIVATE gtest_main)
  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach(TEST_SOURCE_PATH ${TEST_SOURCE_PATHS})
