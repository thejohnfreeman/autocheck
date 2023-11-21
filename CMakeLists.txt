# https://cliutils.gitlab.io/modern-cmake/chapters/intro/newcmake.html
cmake_minimum_required(VERSION 3.7)

project(autocheck
  VERSION 1.0.0
  LANGUAGES CXX
  DESCRIPTION "Header-only C++11 library for property-based testing."
  HOMEPAGE_URL "https://github.com/thejohnfreeman/autocheck"
)

find_package(cupcake 0.4.1 REQUIRED)

cupcake_project(
  LICENSE "ISC"
  AUTHORS "John Freeman <jfreeman08@gmail.com>"
)

cupcake_add_library(autocheck)

cupcake_add_tests()

# add_subdirectory(tutorial)

cupcake_install_project()
cupcake_install_cpp_info()
