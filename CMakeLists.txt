cmake_minimum_required(VERSION 3.3)

# Enable CTest
# Needs to be in top level so tests can be run from top level
enable_testing()

project(engine CXX C)

# Add the project CMake modules to the CMake module path
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

include(ClangFormat)
include(ClangTidy)
include(CppCheck)
include(OcLint)

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

# TODO(Keegan): Think more about required SDL versions
pkg_search_module(SDL2 REQUIRED sdl2 >= 2.0.0)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

# Download and build external third party libraries
include(FetchAnax)
FetchAnax(GITVERSION "d68526bfad15ed6e2f1c66c90eb8993c086e9d30")
include(FetchGtest)
FetchGoogleTest(GITVERSION "a2b8a8e07628e5fd60644b6dd99c1b5e7d7f1f47")
include(FetchJsonCpp)
FetchJsonCpp(GITVERSION "1.7.7")
include(FetchSpdlog)
FetchSpdlog(GITVERSION "v0.11.0")
include(FetchWinkSignals)
FetchWinkSignals(GITVERSION "4165ddb83ca0f33d707e3b7aa6e59349ab08b591")

set(CHECK_SRCS "")

include_directories(inc)

add_subdirectory(src)
add_subdirectory(lib)
add_subdirectory(test)

# Add target to do JSON linting
add_custom_target(json_validate tools/json_check.sh WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
add_dependencies(json_validate jsoncheck)

# Add target to run all linters
add_custom_target (
    lint
    )
add_dependencies(lint clangtidy cppcheck json_validate oclint)

find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based
documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
         message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(doxyfile ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)

    add_custom_target(doc
                      COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                      COMMENT "Generating API documentation with Doxygen"
                      VERBATIM)
endif()
