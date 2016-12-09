# TODO(Keegan): Add option to link statically
function(FetchWinkSignals)
    set(options)
    set(oneValueArgs GITVERSION)
    set(multiValueArgs)
    cmake_parse_arguments(FetchWinkSignals "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
    # Set defaults for unprovided arguments
    if(NOT FetchWinkSignals_GITVERSION)
        set(FetchWinkSignals_GITVERSION "master")
    endif()

    # Enable ExternalProject CMake module
    include(ExternalProject)

    # Download and Install WinkSignals
    # Based off http://www.kaizou.org/2014/11/gtest-cmake/
    # Thanks to David Corvoysier and Henry Schreiner
    # TODO(Keegan): Select version of WinkSignals
    ExternalProject_Add(
        winksignals_proj
        GIT_REPOSITORY https://github.com/important-business/Wink-Signals.git
        GIT_TAG ${FetchWinkSignals_GITVERSION}
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/Wink-Signals
        # Disable install step
        INSTALL_COMMAND ""
        CMAKE_COMMAND ""
    )

    ExternalProject_Get_Property(winksignals_proj source_dir binary_dir)

    # Create an interface library for tests to link against
    add_library(winksignals INTERFACE)
    add_dependencies(winksignals winksignals_proj)
    target_link_libraries(
        winksignals
        INTERFACE
        "${binary_dir}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}winksignals${CMAKE_SHARED_LIBRARY_SUFFIX}"
    )
    set(WINKSIGNALS_INCLUDE_DIR "${source_dir}" PARENT_SCOPE)
endfunction(FetchWinkSignals)
