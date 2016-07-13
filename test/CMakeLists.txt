# Use the FetchGtest module to get gtest library
include(FetchGtest)
FetchGoogleTest()

# Define the list of test modules
# set(test_modules core)
# TODO: Figure out how to link tests to the modules
set(test_modules "")

# Define the test executables
foreach(test_mod ${test_modules})
    file(GLOB test_cases ./${test_mod}/*.cpp)

    # Define an executable for each test module
    set (test_exe_name test_${test_mod})
    add_executable(${test_exe_name} ${test_cases} test_main.cpp)

    # WTF: Why public?
    target_link_libraries(${test_exe_name} PUBLIC gtest ${test_mod})

    # Add the test executable to ctest
    add_test(NAME ${test_exe_name}
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test/${test_exe_name} --gtest_color=yes
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endforeach()
