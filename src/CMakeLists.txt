add_library(libengine core/example_module.cpp)

# Define engine executable
add_executable(engine main.cpp)
target_link_libraries(engine libengine)

# We don't need these yet, but check that they link OK
target_link_libraries(engine ${GLEW_LIBRARIES} ${GLFW_LIBRARIES})
target_include_directories(engine PUBLIC ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})

