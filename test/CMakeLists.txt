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
    URL https://googletest.googlecode.com/files/gtest-1.7.0.zip
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

# Define the list of test modules
set(test_modules core)

# Define the test executables
foreach(test_mod ${test_modules})
    file(GLOB test_cases ./${test_mod}/*.cpp)

    # Define an executable for each test module
    set (test_exe_name test_${test_mod})
    add_executable(${test_exe_name} ${test_cases} test_main.cpp)

    # WTF: Why public?
    target_link_libraries(${test_exe_name} PUBLIC libgtest lib${test_mod})

    # Add the test executables to ctest
    add_test(NAME ${test_exe_name}
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test/${test_exe_name} --gtest_color=yes
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endforeach()
