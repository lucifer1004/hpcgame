if(aitisa_api_public_utils_cmake_included)
  return()
endif()
set(aitisa_api_public_utils_cmake_included true)

##############################################################################
# Macro to update cached options.
##############################################################################
macro (aitisa_api_update_option variable value)
  get_property(_help_string CACHE ${variable} PROPERTY HELPSTRING)
  set(${variable} ${value} CACHE BOOL ${_help_string} FORCE)
endmacro()

##############################################################################
# Fucntion to include some items from list 
##############################################################################
function(exclude output input)
  set(excludes ${ARGN})
  foreach(exclude ${excludes})
    list(REMOVE_ITEM input "${exclude}")
  endforeach()
  set(${output} ${input} PARENT_SCOPE)
endfunction(exclude)

##############################################################################
# This function works around a CMake issue with setting include directories of
# imported libraries built with `ExternalProject_Add`.
# https://gitlab.kitware.com/cmake/cmake/issues/15052
##############################################################################
function(aitisa_api_make_include_dir target)
  get_target_property(DIR ${target} INTERFACE_INCLUDE_DIRECTORIES)
  file(MAKE_DIRECTORY ${DIR})
endfunction()

##############################################################################
# This function encodes relative rpath for exectuables built
# or installed with HICE
##############################################################################
function(aitisa_api_encode_rpath target rel_intall_libdir)
  if(APPLE)
    set(_rpath_portable_origin "@loader_path")
  else()
    set(_rpath_portable_origin "\$ORIGIN")
  endif()
  file(TO_NATIVE_PATH "${_rpath_portable_origin}/../${rel_install_libdir}"
       portable_rpath)
  set_target_properties(target
    PROPERTIES
      MACOSX_RPATH ON
      SKIP_BUILD_RPATH OFF
      BUILD_WITH_INSTALL_RPATH OFF
      INSTALL_RPATH_USE_LINK_PATH ON
      INSTALL_RPATH "${portable_rpath}"
  )
endfunction()

##############################################################################
# Common configuration for tests / test cases on Windows
##############################################################################
function(aitisa_api_maybe_configure_windows_test name kind)
    if(WIN32 OR MINGW)
        string(REPLACE  ";" "\;" PATH "${CTESTCONFIG_PATH};$ENV{PATH}")
        set_property(${kind} ${name} PROPERTY ENVIRONMENT "PATH=${PATH}")
        configure_file(${PROJECT_SOURCE_DIR}/cmake/template.vcxproj.user
            ${name}.vcxproj.user @ONLY)
    endif()
endfunction()

##############################################################################
# Register new executable/test
#   name -- name of the executable
#   srcs -- list of source, if many must be enclosed with ""
#   test -- "test" to mark executable as a test, "" otherwise
#   arg4 -- (optional) list of extra library dependencies
##############################################################################
function(aitisa_api_register_exe name srcs test)
    add_executable(${name} ${srcs})
    target_link_libraries(${name} ${ARGN})
    if("${test}" STREQUAL "test")
        add_test(NAME ${name} COMMAND $<TARGET_FILE:${name}>)
        aitisa_api_maybe_configure_windows_test(${name} TEST)
    endif()
endfunction()

##############################################################################
# Register new example
#   name -- name of the executable
#   srcs -- list of source, if many must be enclosed with ""
##############################################################################
function(aitisa_api_register_example name srcs aitisa_api_install_dir)
  add_executable(${name} ${srcs})
  find_package(aitisa_api CONFIG REQUIRED HINTS "${aitisa_api_install_dir}/cmake")
  target_link_libraries(${name} aitisa_api)
endfunction()

##############################################################################
# This macro make an interface library definition that is dependent on the source.
# Taken from PyTorch
##############################################################################
macro(aitisa_api_make_interface_library src dst)
  add_library(${dst} INTERFACE)
  #add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp
  #  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp
  #  DEPENDS ${src})
  #add_library(${dst} "${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp")
  add_dependencies(${dst} ${src})
  # Depending on the nature of the source library as well as the compiler,
  # determine the needed compilation flags.
  get_target_property(_src_target_type ${src} TYPE)
  # Depending on the type of the source library, we will set up the
  # link command for the specific src library.
  if (${_src_target_type} STREQUAL "STATIC_LIBRARY")
    # In the case of static library, we will need to add whole-static flags.
    if(APPLE)
      target_link_libraries(
          ${dst} INTERFACE -Wl,-force_load,$<TARGET_FILE:${src}>)
    elseif(MSVC)
      # In MSVC, we will add whole archive in default.
      target_link_libraries(
          ${dst} INTERFACE -WHOLEARCHIVE:$<TARGET_FILE:${src}>)
    else()
      # Assume everything else is like gcc
      target_link_libraries(${dst} INTERFACE
          "-Wl,--whole-archive" $<TARGET_FILE:${src}> "-Wl,--no-whole-archive")
    endif()
    # Link all interface link libraries of the src target as well.
    # For static library, we need to explicitly depend on all the libraries
    # that are the dependent library of the source library. Note that we cannot
   # use the populated INTERFACE_LINK_LIBRARIES property, because if one of the
    # dependent library is not a target, cmake creates a $<LINK_ONLY:src> wrapper
    # and then one is not able to find target "src". For more discussions, check
    #   https://gitlab.kitware.com/cmake/cmake/issues/15415
    #   https://cmake.org/pipermail/cmake-developers/2013-May/019019.html
    # Specifically the following quote
    #
    # """
    # For STATIC libraries we can define that the PUBLIC/PRIVATE/INTERFACE keys
    # are ignored for linking and that it always populates both LINK_LIBRARIES
    # LINK_INTERFACE_LIBRARIES.  Note that for STATIC libraries the
    # LINK_LIBRARIES property will not be used for anything except build-order
    # dependencies.
    # """
    get_target_property(_src_target_ilink ${src} INTERFACE_LINK_LIBRARIES)
    if(_src_target_ilink)
      # Only get the INTERFACE_LINK_LIBRARIES when using static imported libraries
      target_link_libraries(${dst} INTERFACE
        $<TARGET_PROPERTY:${src},INTERFACE_LINK_LIBRARIES>)
    else()
      target_link_libraries(${dst} INTERFACE
        $<TARGET_PROPERTY:${src},LINK_LIBRARIES>)
    endif()
  elseif(${_src_target_type} STREQUAL "SHARED_LIBRARY")
    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
      target_link_libraries(${dst} INTERFACE
          "-Wl,--no-as-needed" $<TARGET_FILE:${src}> "-Wl,--as-needed")
    else()
      target_link_libraries(${dst} INTERFACE ${src})
    endif()
    # Link all interface link libraries of the src target as well.
    # For shared libraries, we can simply depend on the INTERFACE_LINK_LIBRARIES
    # property of the target.
    target_link_libraries(${dst} INTERFACE
        $<TARGET_PROPERTY:${src},INTERFACE_LINK_LIBRARIES>)
  else()
    message(FATAL_ERROR
        "You made a CMake build file error: target " ${src}
        " must be of type either STATIC_LIBRARY or SHARED_LIBRARY. However, "
        "I got " ${_src_target_type} ".")
  endif()
  # For all other interface properties, manually inherit from the source target.
  set_target_properties(${dst} PROPERTIES
    INTERFACE_COMPILE_DEFINITIONS
    $<TARGET_PROPERTY:${src},INTERFACE_COMPILE_DEFINITIONS>
    INTERFACE_COMPILE_OPTIONS
    $<TARGET_PROPERTY:${src},INTERFACE_COMPILE_OPTIONS>
    INTERFACE_INCLUDE_DIRECTORIES
    $<TARGET_PROPERTY:${src},INTERFACE_INCLUDE_DIRECTORIES>
    INTERFACE_SYSTEM_INCLUDE_DIRECTORIES
    $<TARGET_PROPERTY:${src},INTERFACE_SYSTEM_INCLUDE_DIRECTORIES>)
endmacro()

########################################################################################################
# An option that the user can select. Can accept condition to control when option is available for user.
# Usage:
#   aitisa_api_option(<option_variable> "doc string" <initial value> [dependencies other_value])
# Taken from mxnet
########################################################################################################
function(aitisa_api_option variable description value)
  if(NOT ARGN)
    option(${variable} "${description}" ${value})
  else()
    include(CMakeDependentOption)
    list(GET ARGN -1 _last_item)
    list(REMOVE_AT ARGN -1)
    cmake_dependent_option(${variable} "${description}" ${value} "${ARGN}" ${_last_item})
  endif()
endfunction()
