# TODO: Add option to link statically
function(FetchSpdlog)
    # Enable ExternalProject CMake module
    include(ExternalProject)

    # Download and Install Spdlog
    # Based off http://www.kaizou.org/2014/11/gtest-cmake/
    # Thanks to David Corvoysier and Henry Schreiner
    # TODO: Select version of Spdlog
    ExternalProject_Add(
        spdlog_proj
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG master
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/spdlog
        # Disable install step
        INSTALL_COMMAND ""
    )

    ExternalProject_Get_Property(spdlog_proj source_dir binary_dir)

    # TODO: Is this the best way to export the include directory?
    set(SPDLOG_INCLUDE_DIR "${source_dir}/include" PARENT_SCOPE)
endfunction(FetchSpdlog)
