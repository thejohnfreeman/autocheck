 cmake_minimum_required(VERSION 3.3)

# Enable ExternalProject CMake module
include(ExternalProject)
# Set default ExternalProject root directory
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)

ExternalProject_Add(
	googlemock
	#	SVN_REPOSITORY http://googlemock.googlecode.com/svn/tags/release-1.7.0
	URL https://googlemock.googlecode.com/files/gmock-1.7.0.zip
	TIMEOUT 30
	#PATCH_COMMAND svn patch ${CMAKE_SOURCE_DIR}/gtest.patch ${CMAKE_BINARY_DIR}/ThirdParty/src/googlemock/gtest
	# Force separate output paths for debug and release builds to allow easy
	# identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
	#CMAKE_ARGS -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
	#			-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
	#			-Dgtest_force_shared_crt=ON
	CMAKE_ARGS -Dgtest_force_shared_crt=ON -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
	# Disable install step
	INSTALL_COMMAND ""
	# Wrap download, configure and build steps in a script to log output
	LOG_DOWNLOAD ON
	LOG_CONFIGURE ON
	LOG_BUILD ON)

# Specify include dir for googlemock and googletest
ExternalProject_Get_Property(googlemock source_dir)
include_directories(${source_dir}/include)
include_directories(${source_dir}/gtest/include)
include_directories(include)

add_executable(apply test/apply.cpp)
add_dependencies(apply googlemock)

add_executable(arbitrary test/arbitrary.cpp)
add_dependencies(arbitrary googlemock)

add_executable(check test/check.cpp)
add_dependencies(check googlemock)

add_executable(generator test/generator.cpp)
add_dependencies(generator googlemock)

add_executable(generator_combinators test/generator_combinators.cpp)
add_dependencies(generator_combinators googlemock)

add_executable(is_one_of test/is_one_of.cpp)
add_dependencies(is_one_of googlemock)

add_executable(largest test/largest.cpp)
add_dependencies(largest googlemock)

add_executable(value test/value.cpp)
add_dependencies(value googlemock)

#tutorials
add_executable(insert_sorted tutorial/insert_sorted/insert_sorted.cpp)
add_dependencies(insert_sorted googlemock)

add_executable(reverse tutorial/reverse/reverse.cpp)
add_dependencies(reverse googlemock)

ExternalProject_Get_Property(googlemock binary_dir)
set(EXTRA_LIBRARIES 
		${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gmock${CMAKE_FIND_LIBRARY_SUFFIXES} 
		${binary_dir}/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES})

if( MSVC )
else()
	set(EXTRA_LIBRARIES ${EXTRA_LIBRARIES} rt pthread)
endif()

target_link_libraries(apply ${EXTRA_LIBRARIES})
target_link_libraries(arbitrary ${EXTRA_LIBRARIES})
target_link_libraries(check ${EXTRA_LIBRARIES})
target_link_libraries(generator ${EXTRA_LIBRARIES})
target_link_libraries(generator_combinators ${EXTRA_LIBRARIES})
target_link_libraries(is_one_of ${EXTRA_LIBRARIES})
target_link_libraries(largest ${EXTRA_LIBRARIES})
target_link_libraries(value ${EXTRA_LIBRARIES})
target_link_libraries(insert_sorted ${EXTRA_LIBRARIES})
target_link_libraries(reverse ${EXTRA_LIBRARIES})

enable_testing()
add_test(apply apply)
add_test(arbitrary arbitrary)
add_test(check check)
add_test(generator generator)
add_test(generator_combinators generator_combinators)
add_test(is_one_of is_one_of)
add_test(largest largest)
add_test(value value)
