add_custom_target (
    build_test
    ALL
)

# Allow all children to headers relative to the src directory
include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(core)
add_subdirectory(systems)

# Define an executable for engine test
set(SRCS_TEST_ENGINE
    ${SRCS_TEST_CORE}
    ${SRCS_TEST_SYSTEMS}
    ${CMAKE_CURRENT_SOURCE_DIR}/test_main.cpp
    )

add_executable(test_engine ${SRCS_TEST_ENGINE} ${SRCS_ENGINE_MODULE})

# Link to google test and use its include directories
target_link_libraries(test_engine gtest)
target_include_directories(test_engine PUBLIC ${GTEST_INCLUDE_DIR})

target_link_libraries(test_engine sdl_wrap)

# Link to libanax and use its include directories
target_link_libraries(test_engine anax)
target_include_directories(test_engine PUBLIC ${ANAX_INCLUDE_DIR})

# Use jsoncpp include directories
target_link_libraries(test_engine jsoncpp)
target_include_directories(test_engine PUBLIC ${JSONCPP_INCLUDE_DIR})

# Use wink signals include directories (no library file)
target_include_directories(test_engine SYSTEM PUBLIC ${WINKSIGNALS_INCLUDE_DIR})
add_dependencies(test_engine winksignals)

# Use spdlog include directories (no library file)
target_include_directories(test_engine PUBLIC ${SPDLOG_INCLUDE_DIR})
add_dependencies(test_engine spdlog)

# Link to SDL2 and modules and use their include directories
target_link_libraries(test_engine ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
target_include_directories(test_engine PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

add_dependencies(build_test test_engine)
get_target_property(INCLUDEDIRS_TEST test_engine INCLUDE_DIRECTORIES)
get_target_property(CXX_STANDARD_TEST test_engine CXX_STANDARD)

ClangTidy(
    TARGETNAME test_engine
    FILES ${SRCS_TEST_ENGINE}
    INCLUDEDIRS ${INCLUDEDIRS_TEST}
    CXX_STANDARD ${CXX_STANDARD_TEST}
    )
ClangFormat(
    TARGETNAME test_engine
    FILES ${SRCS_TEST_ENGINE}
    )
CppCheck(
    TARGETNAME test_engine
    FILES ${SRCS_TEST_ENGINE}
    INCLUDEDIRS ${INCLUDEDIRS_TEST}
    )
OcLint(
    TARGETNAME test_engine
    FILES ${SRCS_TEST_ENGINE}
    INCLUDEDIRS ${INCLUDEDIRS_TEST}
    CXX_STANDARD ${CXX_STANDARD_TEST}
    )

# Add the test executable to ctest
add_test(NAME test_engine
COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test/test_engine --gtest_color=yes
WORKING_DIRECTORY ${PROJECT_BINARY_DIR})

