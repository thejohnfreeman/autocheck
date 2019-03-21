add_library(autocheck INTERFACE)
add_library(autocheck::autocheck ALIAS autocheck)
target_include_directories(
  autocheck
  INTERFACE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_compile_features(autocheck INTERFACE cxx_lambdas)
install(
  TARGETS autocheck
  EXPORT ${PROJECT_NAME}-targets
)
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/autocheck
  DESTINATION include
)
