 cmake_minimum_required(VERSION 2.8)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # using Clang
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -O0 -g3 -ggdb")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W3 /Od")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox")
endif()

include(ExternalProject)
enable_testing()

# Set default ExternalProject root directory
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)

ExternalProject_Add(
    googlemock
    URL https://googlemock.googlecode.com/files/gmock-1.7.0.zip
    TIMEOUT 30
    CMAKE_ARGS -Dgtest_force_shared_crt=ON -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)

ExternalProject_Get_Property(googlemock source_dir)
ExternalProject_Get_Property(googlemock binary_dir)

if( MSVC )
set(EXTERNAL_LIBRARIES 
    ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gmock${CMAKE_FIND_LIBRARY_SUFFIXES} 
    ${binary_dir}/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES})
else()
    set(EXTERNAL_LIBRARIES 
	${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gmock.a 
	${binary_dir}/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main.a
	rt pthread)
endif()

include_directories(${source_dir}/include)
include_directories(${source_dir}/gtest/include)
include_directories(include)

#Add the Catch C++ test library
ExternalProject_Add(
    catch
    URL https://github.com/philsquared/Catch/archive/v1.2.1.zip
    TIMEOUT 10
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
   )

# Expose required variable (CATCH_INCLUDE_DIR) to parent scope
ExternalProject_Get_Property(catch source_dir)
message("Include: ${source_dir}")
set(CATCH_INCLUDE_DIR ${source_dir}/include CACHE INTERNAL "Path to include folder for Catch")
include_directories(${CATCH_INCLUDE_DIR})

file( GLOB APP_SOURCES test/*.cpp)
foreach( testsourcepath ${APP_SOURCES} )
	get_filename_component(testsourcefile ${testsourcepath} NAME)
    # I used a simple string replace, to cut off .cpp.
    string( REPLACE ".cpp" "" testname ${testsourcefile} )
    add_executable( ${testname} ${testsourcepath} )
	target_link_libraries( ${testname} ${EXTERNAL_LIBRARIES})
	add_dependencies(${testname} googlemock)
	add_dependencies(${testname} catch)
	add_test(${testname} ${testname})
endforeach( testsourcepath ${APP_SOURCES} )

#tutorials
add_executable(insert_sorted tutorial/insert_sorted/insert_sorted.cpp)
add_dependencies(insert_sorted googlemock)

add_executable(reverse tutorial/reverse/reverse.cpp)
add_dependencies(reverse googlemock)

add_executable(factorial tutorial/factorial/factorial.cpp)
add_dependencies(factorial catch)

add_executable(demonstrate_failure tutorial/demonstrate_failure/demonstrate_failure.cpp)
add_dependencies(demonstrate_failure googlemock)
target_link_libraries(demonstrate_failure ${EXTERNAL_LIBRARIES})
