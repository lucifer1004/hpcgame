cmake_minimum_required(VERSION 3.1)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    project(answer)

    set(XSIMD_INCLUDE_DIR xsimd)
endif()

include_directories(${XSIMD_INCLUDE_DIR})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Intel")
    if(NOT CMAKE_CXX_FLAGS MATCHES "-march" AND NOT CMAKE_CXX_FLAGS MATCHES "-arch" AND NOT CMAKE_OSX_ARCHITECTURES)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -mtune=native")
    endif()

    if(NOT CMAKE_CXX_COMPILER_ID MATCHES Clang) # We are using clang-cl
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
    endif()
endif()

add_executable(answer answer.cpp ${XSIMD_HEADERS})
target_link_libraries(answer xsimd)
target_compile_features(answer PRIVATE cxx_std_17)