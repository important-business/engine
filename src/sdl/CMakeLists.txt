# Define sources
set(SRCS_SDL wrap.cpp)

# Define library
add_library(libsdl SHARED ${SRCS_SDL})

target_link_libraries(libsdl ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
target_include_directories(libsdl PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
