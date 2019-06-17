# Need 3.1 for C++11.
# Need 3.7 for future.
# Need 3.12 for CONFIGURE_DEPENDS.
# https://cliutils.gitlab.io/modern-cmake/chapters/intro/newcmake.html
cmake_minimum_required(VERSION 3.7)
project(autocheck
  VERSION 1.0.0
  LANGUAGES CXX
  DESCRIPTION "Header-only C++11 library for property-based testing."
  HOMEPAGE_URL "https://github.com/thejohnfreeman/autocheck"
)

set(CMAKE_FIND_PACKAGE_SORT_ORDER NATURAL)
set(CMAKE_FIND_PACKAGE_SORT_DIRECTION DEC)
find_package(future CONFIG REQUIRED)

future_project(
  LICENSE "ISC"
  AUTHORS "John Freeman <jfreeman08@gmail.com>"
)

future_add_dependency(PRIVATE GTest CONFIG)
target_compile_options(GTest::gtest INTERFACE "-D_GLIBCXX_USE_CXX11_ABI=0")
future_add_dependency(PRIVATE Catch2)

add_subdirectory(include)
enable_testing()
add_subdirectory(tests)
add_subdirectory(tutorial)

future_install_project()
