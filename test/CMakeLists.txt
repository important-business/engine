set(test_modules "core;systems")

include_directories(${CMAKE_SOURCE_DIR}/src/)

add_custom_target (
    build_test
    ALL
)

# Define the test executables
foreach(test_mod ${test_modules})

    add_subdirectory(${test_mod})

    # Define an executable for each test module
    set(test_exe_name test_${test_mod})
    set(SRCS_TESTMOD ${SRCS_TESTMOD} ${CMAKE_CURRENT_SOURCE_DIR}/test_main.cpp)
    add_executable(${test_exe_name} ${SRCS_TESTMOD})

    # Link to google test and use its include directories
    target_link_libraries(${test_exe_name} gtest)
    target_include_directories(${test_exe_name} PUBLIC ${GTEST_INCLUDE_DIR})

    target_link_libraries(${test_exe_name} sdl_wrap)

    # Link to libanax and use its include directories
    target_link_libraries(${test_exe_name} anax)
    target_include_directories(${test_exe_name} PUBLIC ${ANAX_INCLUDE_DIR})

    # Use jsoncpp include directories
    target_link_libraries(${test_exe_name} jsoncpp)
    target_include_directories(${test_exe_name} PUBLIC ${JSONCPP_INCLUDE_DIR})

    # Use wink signals include directories (no library file)
    target_include_directories(${test_exe_name} SYSTEM PUBLIC ${WINKSIGNALS_INCLUDE_DIR})
    add_dependencies(${test_exe_name} winksignals)

    # Use spdlog include directories (no library file)
    target_include_directories(${test_exe_name} PUBLIC ${SPDLOG_INCLUDE_DIR})
    add_dependencies(${test_exe_name} spdlog)

    # Link to SDL2 and modules and use their include directories
    target_link_libraries(${test_exe_name} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
    target_include_directories(${test_exe_name} PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

    add_dependencies(build_test ${test_exe_name})
    get_target_property(INCLUDEDIRS_TEST ${test_exe_name} INCLUDE_DIRECTORIES)
    get_target_property(CXX_STANDARD_TEST ${test_exe_name} CXX_STANDARD)

    ClangTidy(
        TARGETNAME ${test_exe_name}
        FILES ${SRCS_TESTMOD}
        INCLUDEDIRS ${INCLUDEDIRS_TEST}
        CXX_STANDARD ${CXX_STANDARD_TEST}
        )
    ClangFormat(
        TARGETNAME ${test_exe_name}
        FILES ${SRCS_TESTMOD}
        )

    # Add the test executable to ctest
    add_test(NAME ${test_exe_name}
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test/${test_exe_name} --gtest_color=yes
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})

endforeach()
