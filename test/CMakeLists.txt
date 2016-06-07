# Required for google test
find_package(Threads REQUIRED)

# Enable ExternalProject CMake module
include(ExternalProject)

# Download and install GoogleTest
ExternalProject_Add(
    gtest
    URL https://googletest.googlecode.com/files/gtest-1.7.0.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    # Disable install step
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(gtest source_dir binary_dir)

add_library(libgtest INTERFACE)
add_dependencies(libgtest gtest)
target_link_libraries(libgtest INTERFACE Threads::Threads "${binary_dir}/libgtest.a" "${binary_dir}/libgtest_main.a")
target_include_directories(libgtest INTERFACE "${source_dir}/include")

enable_testing()

file(GLOB test_cases ./**/*.cpp)
foreach(case_file ${test_cases})
    get_filename_component( case_name ${case_file} NAME_WE )
    set (case_name test_${case_name})
    add_executable(${case_name} ${case_file} test_main.cpp)
    target_link_libraries(${case_name} PUBLIC libgtest libengine)
    add_test(NAME ${case_name}
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test/${case_name}
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endforeach()
