include(FetchAnax)
FetchAnax(GITVERSION "d68526bfad15ed6e2f1c66c90eb8993c086e9d30")
include(FetchSpdlog)
FetchSpdlog()

# Allow all children to headers relative to the src directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# This is temporary, should be by component to enforce isolation
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

# Load CMakeLists for modules
add_subdirectory(core)
add_subdirectory(sdl_wrap)
add_subdirectory(components)
add_subdirectory(systems)

# Define engine executable
# TODO: Add SRCS_COMPONENTS once components with cpp files are added
set(SRCS_ENGINE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${SRCS_CORE} ${SRCS_SYSTEMS})
add_executable(engine ${SRCS_ENGINE})


# TODO: Link to components once they have been added
# Link to local modules
target_link_libraries(engine sdl_wrap)

# Link to libanax and use its include directories
target_link_libraries(engine anax)
target_include_directories(engine PUBLIC ${ANAX_INCLUDE_DIR})

# Use spdlog include directories (no library file)
target_include_directories(engine PUBLIC ${SPDLOG_INCLUDE_DIR})
add_dependencies(engine spdlog)

# Link to SDL2 and modules and use their include directories
target_link_libraries(engine ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
target_include_directories(engine PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

include(ClangTidy)
get_target_property(INCLUDEDIRS_ENGINE engine INCLUDE_DIRECTORIES)
get_target_property(CXX_STANDARD_ENGINE engine CXX_STANDARD)
ClangTidy(TARGETNAME engine FILES ${SRCS_ENGINE} INCLUDEDIRS ${INCLUDEDIRS_ENGINE} CXX_STANDARD ${CXX_STANDARD_ENGINE})
