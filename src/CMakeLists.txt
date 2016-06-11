# Load CMakeLists for modules
add_subdirectory(core)

# Define engine executable
add_executable(engine main.cpp)
target_link_libraries(engine libcore)

# We don't need these yet, but check that they link OK
target_link_libraries(engine ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
target_include_directories(engine PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

