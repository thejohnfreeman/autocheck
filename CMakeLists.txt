# Need 3.1 for C++11.
# Need 3.12 for CONFIGURE_DEPENDS.
# https://cliutils.gitlab.io/modern-cmake/chapters/intro/newcmake.html
cmake_minimum_required(VERSION 3.1)
project(autocheck VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
find_package(gtest)

add_subdirectory(include)
enable_testing()
add_subdirectory(tests)
