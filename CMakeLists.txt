cmake_minimum_required(VERSION 3.3)

# Aquire external libraries
INCLUDE(FindPkgConfig)

enable_testing()
project(engine CXX)

include(${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)
include(CheckCXXSourceCompiles)

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
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(CMAKE_CXX_FLAGS_DEBUG "/Zi /FS /DEBUG /Od /MDd")
	set(CMAKE_CXX_FLAGS_RELEASE "/Ox -DNDEBUG")
else()
    message(SEND_ERROR "Unrecognized compiler")
endif()

# Add packageconfig libraries, use the following to take advantage of these
# target_link_libraries(example ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
# target_include_directories(example PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

include_directories(inc)

add_subdirectory(src)
add_subdirectory(lib)

