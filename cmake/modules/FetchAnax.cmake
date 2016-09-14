# TODO: Add option to link statically
function(FetchAnax)
    set(options)
    set(oneValueArgs GITVERSION)
    set(multiValueArgs)
    cmake_parse_arguments(FetchAnax "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
    # Set defaults for unprovided arguments
    if(NOT FetchAnax_GITVERSION)
        set(FetchAnax_GITVERSION "master")
    endif()

    # Enable ExternalProject CMake module
    include(ExternalProject)

    # Download and Install Anax
    # Based off http://www.kaizou.org/2014/11/gtest-cmake/
    # Thanks to David Corvoysier and Henry Schreiner
    # TODO: Select version of Anax
    ExternalProject_Add(
        anax_proj
        GIT_REPOSITORY https://github.com/miguelmartin75/anax.git
        GIT_TAG ${FetchAnax_GITVERSION}
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/anax
        # Disable install step
        INSTALL_COMMAND ""
    )

    ExternalProject_Get_Property(anax_proj source_dir binary_dir)

    # Create an interface library for tests to link against
    add_library(anax INTERFACE)
    add_dependencies(anax anax_proj)
    target_link_libraries(
        anax
        INTERFACE
        "${binary_dir}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}anax${CMAKE_SHARED_LIBRARY_SUFFIX}"
    )
    # TODO: Is this the best way to export the include directory?
    set(ANAX_INCLUDE_DIR "${source_dir}/include" "${binary_dir}/include" PARENT_SCOPE)
endfunction(FetchAnax)
