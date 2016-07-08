# Define sources
set(SRCS_SYSTEMS rendersystem.cpp)

# Define library
add_library(systems STATIC ${SRCS_SYSTEMS})

# Add Anax to include directories
target_include_directories(systems PUBLIC ${ANAX_INCLUDE_DIR})

