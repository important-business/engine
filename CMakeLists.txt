cmake_minimum_required(VERSION 3.3)

# Enable CTest
# Needs to be in top level so tests can be run from top level
enable_testing()

project(engine CXX C)

# Add the project CMake modules to the CMake module path
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

#Do debug build by default
if (NOT CMAKE_BUILD_TYPE)
    message("-- Defaulting to debug build (use -DCMAKE_BUILD_TYPE:STRING=Release for release build)")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Use C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED_ON)

# Set up per-compiler arguments
# Based off of entityx
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "(GNU|.*Clang)")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Werror -Wall -Wextra -Wno-unused-parameter -Wno-error=unused-variable -Wno-error=sign-compare")
	set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
	set(CMAKE_CXX_FLAGS_RELEASE "-g -O2 -DNDEBUG")
else()
    message(SEND_ERROR "Unrecognized compiler")
endif()

# Aquire external libraries
find_package(PkgConfig REQUIRED)

# TODO(Keegan) Think more about required SDL versions
pkg_search_module(SDL2 REQUIRED sdl2 >= 2.0.0)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

set(CHECK_SRCS "")

include_directories(inc)

add_subdirectory(src)
add_subdirectory(lib)
add_subdirectory(test)

# Based off of https://arcanis.me/en/2015/10/17/cppcheck-and-clang-format/
# Find project source files, excluding files in the build directory
file(GLOB_RECURSE CHECK_SRCS *.cpp *.hpp *.c *.h)
foreach (SOURCE_FILE ${CHECK_SRCS})
    string(FIND ${SOURCE_FILE} ${PROJECT_BINARY_DIR} PROJECT_BIN_DIR_FOUND)
    if (NOT ${PROJECT_BIN_DIR_FOUND} EQUAL -1)
        list(REMOVE_ITEM CHECK_SRCS ${SOURCE_FILE})
    endif ()
endforeach ()

include(ClangFormat)
ClangFormat(CLANGFORMAT_FILES ${CHECK_SRCS})

