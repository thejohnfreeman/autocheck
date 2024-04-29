# https://cliutils.gitlab.io/modern-cmake/chapters/intro/newcmake.html
cmake_minimum_required(VERSION 3.20)

project(autocheck
  VERSION 1.0.0
  LANGUAGES CXX
  DESCRIPTION "Header-only C++17 library for property-based testing."
  HOMEPAGE_URL "https://github.com/thejohnfreeman/autocheck"
)

find_package(cupcake.cmake REQUIRED)
cupcake_project(
  LICENSE "ISC"
  AUTHORS "John Freeman <jfreeman08@gmail.com>"
)
cupcake_add_libraries()
cupcake_enable_testing()
# add_subdirectory(tutorial)
cupcake_install_project()
cupcake_install_cpp_info()
