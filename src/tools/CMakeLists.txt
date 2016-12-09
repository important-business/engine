# Define jsoncheck executable
# TODO(Keegan): Add SRCS_COMPONENTS once components with cpp files are added
set(SRCS_JSONCHECK
    ${CMAKE_CURRENT_SOURCE_DIR}/jsoncheck.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/exception.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/logging.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/json.cpp
    )
add_executable(jsoncheck ${SRCS_JSONCHECK})

# Use jsoncpp include directories
target_link_libraries(jsoncheck jsoncpp)
target_include_directories(jsoncheck PUBLIC ${JSONCPP_INCLUDE_DIR})

# Link to google test and use its include directories
target_link_libraries(jsoncheck gtest)
target_include_directories(jsoncheck PUBLIC ${GTEST_INCLUDE_DIR})

# Use spdlog include directories (no library file)
target_include_directories(jsoncheck PUBLIC ${SPDLOG_INCLUDE_DIR})
add_dependencies(jsoncheck spdlog)

get_target_property(INCLUDEDIRS_JSONCHECK jsoncheck INCLUDE_DIRECTORIES)
get_target_property(CXX_STANDARD_JSONCHECK jsoncheck CXX_STANDARD)

ClangTidy(
    TARGETNAME jsoncheck
    FILES ${SRCS_JSONCHECK}
    INCLUDEDIRS ${INCLUDEDIRS_JSONCHECK}
    CXX_STANDARD ${CXX_STANDARD_JSONCHECK}
    )
ClangFormat(
    TARGETNAME jsoncheck
    FILES ${SRCS_JSONCHECK}
    )
