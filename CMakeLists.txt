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

# Set up per-compiler arguments
# Based off of entityx
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "(GNU|.*Clang)")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Werror -Wall -Wextra -Wno-unused-parameter -Wno-error=unused-variable -Wno-error=sign-compare -std=c++11")
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

include_directories(inc)

add_subdirectory(src)
add_subdirectory(lib)
add_subdirectory(test)

