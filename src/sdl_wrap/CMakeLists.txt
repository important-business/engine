# Define sources
set(SRCS_SDL_WRAP wrap.cpp)

# Define library
add_library(sdl_wrap STATIC ${SRCS_SDL_WRAP})

target_link_libraries(sdl_wrap ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
target_include_directories(sdl_wrap PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

# Use spdlog include directories (no library file)
target_include_directories(sdl_wrap PUBLIC ${SPDLOG_INCLUDE_DIR})
add_dependencies(sdl_wrap spdlog)
