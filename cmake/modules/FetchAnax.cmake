# TODO(Keegan): Add option to link statically
function(FetchAnax)
    set(options)
    set(oneValueArgs GITVERSION BUILD_SHARED)
    set(multiValueArgs)
    cmake_parse_arguments(FetchAnax "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
    # Set defaults for unprovided arguments
    if(NOT FetchAnax_GITVERSION)
        set(FetchAnax_GITVERSION "master")
    endif()
    if(FetchAnax_BUILD_SHARED STREQUAL "")
        set(FetchAnax_BUILD_SHARED "false")
    endif()

    # Enable ExternalProject CMake module
    include(ExternalProject)

    # Download and Install Anax
    # Based off http://www.kaizou.org/2014/11/gtest-cmake/
    # Thanks to David Corvoysier and Henry Schreiner
    # TODO(Keegan): Select version of Anax
    ExternalProject_Add(
        anax_proj
        GIT_REPOSITORY https://github.com/miguelmartin75/anax.git
        GIT_TAG ${FetchAnax_GITVERSION}
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/anax
        # Disable install step
        INSTALL_COMMAND ""
        CMAKE_CACHE_ARGS "-DBUILD_SHARED_LIBS:BOOL=${FetchAnax_BUILD_SHARED}"
    )

    if(FetchAnax_BUILD_SHARED)
        set(LIBRARY_SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX})
        set(LIBRARY_PREFIX "/lib/${CMAKE_SHARED_LIBRARY_PREFIX}")
    else()
        set(LIBRARY_SUFFIX ${CMAKE_STATIC_LIBRARY_SUFFIX})
        set(LIBRARY_PREFIX ${CMAKE_STATIC_LIBRARY_PREFIX})
    endif()
    ExternalProject_Get_Property(anax_proj source_dir binary_dir)

    # Create an interface library for tests to link against
    add_library(anax INTERFACE)
    add_dependencies(anax anax_proj)

    target_link_libraries(
        anax
        INTERFACE
        "${binary_dir}/${LIBRARY_PREFIX}anax${LIBRARY_SUFFIX}"
    )
    # TODO(Keegan: Is this the best way to export the include directory?
    set(ANAX_INCLUDE_DIR "${source_dir}/include" "${binary_dir}/include" PARENT_SCOPE)
endfunction(FetchAnax)
