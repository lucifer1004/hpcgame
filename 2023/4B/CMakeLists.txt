cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(aitisa_api VERSION 0.4.0 LANGUAGES C CXX)

##############################################
# General options
##############################################
option(AITISA_API_BUILD_SHARED_LIBS "Compiled as a shared library" ON)
option(AITISA_API_BUILD_TESTING "Build with testing enabled" ON)

if(AITISA_API_BUILD_SHARED_LIBS)
  set(BUILD_SHARED_LIBS ON CACHE INTERNAL "")
endif()

# Show the detail of compile messages
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_COLOR_MAKEFILE   ON)

##############################################
# General setup
##############################################
# https://blog.kitware.com/cmake-and-the-default-build-type
#set(default_build_type "Release")
set(default_build_type "Debug")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
    STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(AITISA_API_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(AITISA_API_CMAKE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(AITISA_API_EXTERNAL_DIR ${AITISA_API_ROOT_DIR}/third_party)


include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

##############################################
# Create target and set properties
##############################################

include(cmake/public/Utils.cmake)

# Find all the files under `src/`
FILE(GLOB_RECURSE aitisa_api_srcs "src/*.c" "src/*.h")

# Create library target with all the source codes.
if(BUILD_SHARED_LIBS)
  add_library(aitisa_api SHARED ${aitisa_api_srcs})
  set_target_properties(aitisa_api
    PROPERTIES
      INTERFACE_POSITION_INDEPENDENT_CODE ON
      OUTPUT_NAME "aitisa_api"
      SOVERSION ${PROJECT_VERSION_MAJOR}
      C_VISIBILITY_PRESET "hidden"
      VISIBILITY_INLINES_HIDDEN ON
  )
  target_compile_definitions(aitisa_api
    PRIVATE
      AITISA_API_SHARED_LIBS
      AITISA_API_SHARED_LIBS_EXPORTS
  )
else()
  add_library(aitisa_api STATIC ${aitisa_api_srcs})
endif()

target_compile_features(aitisa_api PUBLIC c_std_99)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
target_include_directories(aitisa_api
  PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

aitisa_api_make_interface_library(aitisa_api aitisa_api_interface)
add_library(aitisa_api::aitisa_api ALIAS aitisa_api_interface)

##############################################
# Testing
##############################################
if(AITISA_API_BUILD_TESTING)
  add_subdirectory(test)
endif()
