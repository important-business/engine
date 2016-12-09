# Allow all children to headers relative to the src directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# This is temporary, should be by component to enforce isolation
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

# Load CMakeLists for modules
add_subdirectory(core)
add_subdirectory(sdl_wrap)
add_subdirectory(components)
add_subdirectory(systems)
add_subdirectory(tools)

# Define engine executable
set(SRCS_ENGINE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${SRCS_CORE}
    ${SRCS_SYSTEMS}
    ${SRCS_COMPONENTS}
    )
add_executable(engine ${SRCS_ENGINE})

# Link to local modules
target_link_libraries(engine sdl_wrap)

# Link to libanax and use its include directories
target_link_libraries(engine anax)
target_include_directories(engine PUBLIC ${ANAX_INCLUDE_DIR})

# Link to google test and use its include directories
target_link_libraries(engine gtest)
target_include_directories(engine PUBLIC ${GTEST_INCLUDE_DIR})

# Use jsoncpp include directories
target_link_libraries(engine jsoncpp)
target_include_directories(engine PUBLIC ${JSONCPP_INCLUDE_DIR})

# Use wink signals include directories (no library file)
target_include_directories(engine SYSTEM PUBLIC ${WINKSIGNALS_INCLUDE_DIR})
add_dependencies(engine winksignals)

# Use spdlog include directories (no library file)
target_include_directories(engine PUBLIC ${SPDLOG_INCLUDE_DIR})
add_dependencies(engine spdlog)

# Link to SDL2 and modules and use their include directories
target_link_libraries(engine ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
target_include_directories(engine PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

get_target_property(INCLUDEDIRS_ENGINE engine INCLUDE_DIRECTORIES)
get_target_property(CXX_STANDARD_ENGINE engine CXX_STANDARD)
ClangTidy(TARGETNAME engine FILES ${SRCS_ENGINE} INCLUDEDIRS ${INCLUDEDIRS_ENGINE} CXX_STANDARD ${CXX_STANDARD_ENGINE})
ClangFormat(TARGETNAME engine FILES ${SRCS_ENGINE})
