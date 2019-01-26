# Need 3.1 for C++11.
# Need 3.12 for CONFIGURE_DEPENDS.
# https://cliutils.gitlab.io/modern-cmake/chapters/intro/newcmake.html
cmake_minimum_required(VERSION 3.1)
project(autocheck VERSION 1.0.0 LANGUAGES CXX)

# Following the CMake wiki to select a static runtime just like googletest.
# https://gitlab.kitware.com/cmake/community/wikis/FAQ#dynamic-replace
# A setting for this has been requested for at least 9 years.
# https://gitlab.kitware.com/cmake/cmake/issues/9445
foreach(flag
    CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
    CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
  if(${flag} MATCHES "/MD")
    string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
  endif(${flag} MATCHES "/MD")
endforeach(flag)

add_subdirectory(external/googletest)
add_subdirectory(include)
enable_testing()
add_subdirectory(tests)
