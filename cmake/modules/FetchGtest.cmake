# Required for google test
find_package(Threads REQUIRED)

# Enable ExternalProject CMake module
include(ExternalProject)

# Download and install GoogleTest
# Based off http://www.kaizou.org/2014/11/gtest-cmake/
# Thanks to David Corvoysier and Henry Schreiner
# TODO: Update Gtest?
ExternalProject_Add(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.7.0
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    # Disable install step
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(gtest source_dir binary_dir)

# Create an interface library for tests to link against
add_library(libgtest INTERFACE)
add_dependencies(libgtest gtest)
target_link_libraries(libgtest INTERFACE Threads::Threads "${binary_dir}/libgtest.a")
target_include_directories(libgtest INTERFACE "${source_dir}/include")

