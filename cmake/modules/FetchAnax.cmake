# TODO: Add option to link statically
function(FetchAnax)
    # Enable ExternalProject CMake module
    include(ExternalProject)

    # Download and Install Anax
    # Based off http://www.kaizou.org/2014/11/gtest-cmake/
    # Thanks to David Corvoysier and Henry Schreiner
    # TODO: Select version of Anax
    ExternalProject_Add(
        anax_proj
        GIT_REPOSITORY https://github.com/miguelmartin75/anax.git
        GIT_TAG v2.1.0
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
    target_include_directories(anax INTERFACE "${source_dir}/include")
endfunction(FetchAnax)
