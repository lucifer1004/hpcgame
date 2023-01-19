if(aitisa_api_public_gtest_cmake_included)
  return()
endif()
set(aitisa_api_public_gtest_cmake_included true)

aitisa_api_update_option(GTEST_ROOT ${GTEST_ROOT_DIR})

find_package(GTEST REQUIRED)
add_library(aitisa_api::gtest INTERFACE IMPORTED)
set_property(
  TARGET aitisa_api::gtest PROPERTY INTERFACE_INCLUDE_DIRECTORIES
  ${GTEST_INCLUDE_DIRS})
set_property(
  TARGET aitisa_api::gtest PROPERTY INTERFACE_LINK_LIBRARIES
  ${GTEST_BOTH_LIBRARIES})