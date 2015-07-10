 cmake_minimum_required(VERSION 2.8)

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
set(EXTRA_LIBRARIES 
    ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gmock${CMAKE_FIND_LIBRARY_SUFFIXES} 
    ${binary_dir}/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES})

if( MSVC )
else()
    set(EXTRA_LIBRARIES ${EXTRA_LIBRARIES} rt pthread)
endif()

include_directories(${source_dir}/include)
include_directories(${source_dir}/gtest/include)
include_directories(include)

#Add the Catch C++ test library
find_package(Git REQUIRED)

ExternalProject_Add(
    catch
    PREFIX ${CMAKE_BINARY_DIR}/catch
    GIT_REPOSITORY https://github.com/philsquared/Catch.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
   )

# Expose required variable (CATCH_INCLUDE_DIR) to parent scope
ExternalProject_Get_Property(catch source_dir)
set(CATCH_INCLUDE_DIR ${source_dir}/include CACHE INTERNAL "Path to include folder for Catch")
include_directories(${CATCH_INCLUDE_DIR})

file( GLOB APP_SOURCES test/*.cpp)
foreach( testsourcepath ${APP_SOURCES} )
	get_filename_component(testsourcefile ${testsourcepath} NAME)
    # I used a simple string replace, to cut off .cpp.
    string( REPLACE ".cpp" "" testname ${testsourcefile} )
    add_executable( ${testname} ${testsourcepath} )
	target_link_libraries( ${testname} ${EXTRA_LIBRARIES})
	add_dependencies(${testname} googlemock)
	add_test(${testname} ${testname})
endforeach( testsourcepath ${APP_SOURCES} )

#tutorials
add_executable(insert_sorted tutorial/insert_sorted/insert_sorted.cpp)
add_dependencies(insert_sorted googlemock)

add_executable(reverse tutorial/reverse/reverse.cpp)
add_dependencies(reverse googlemock)

add_executable(factorial tutorial/factorial.cpp)
add_dependencies(factorial catch)

add_executable(catch_check catch_tests/check.cpp)
add_dependencies(catch_check catch)
