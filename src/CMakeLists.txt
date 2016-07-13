include(FetchAnax)
FetchAnax()

# Allow all children to headers relative to the src directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# This is temporary, should be by component to enforce isolation
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

# Load CMakeLists for modules
add_subdirectory(core)
add_subdirectory(sdl)
add_subdirectory(components)
add_subdirectory(systems)

# Define engine executable
# TODO: Add SRCS_COMPONENTS once components with cpp files are added
add_executable(engine main.cpp ${SRCS_CORE} ${SRCS_SYSTEMS})

# TODO: Link to components once they have been added
# Link to local modules
target_link_libraries(engine sdl)

# Link to libanax and use its include directories
target_link_libraries(engine anax)
target_include_directories(engine PUBLIC ${ANAX_INCLUDE_DIR})

# Link to SDL2 and modules and use their include directories
target_link_libraries(engine ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
target_include_directories(engine PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

