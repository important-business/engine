# TODO(Keegan): Add option to link statically
function(FetchSpdlog)
    set(options)
    set(oneValueArgs GITVERSION)
    set(multiValueArgs)
    cmake_parse_arguments(FetchSpdlog "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
    # Set defaults for unprovided arguments
    if(NOT FetchSpdlog_GITVERSION)
        set(FetchSpdlog_GITVERSION "master")
    endif()

    # Enable ExternalProject CMake module
    include(ExternalProject)

    # Download and Install Spdlog
    # Based off http://www.kaizou.org/2014/11/gtest-cmake/
    # Thanks to David Corvoysier and Henry Schreiner
    # TODO(Keegan): Select version of Spdlog
    ExternalProject_Add(
        spdlog_proj
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG ${FetchSpdlog_GITVERSION}
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/spdlog
        # Disable install step
        INSTALL_COMMAND ""
    )

    ExternalProject_Get_Property(spdlog_proj source_dir binary_dir)

    add_library(spdlog INTERFACE)
    add_dependencies(spdlog spdlog_proj)

    # TODO(Keegan): Is this the best way to export the include directory?
    set(SPDLOG_INCLUDE_DIR "${source_dir}/include" PARENT_SCOPE)
endfunction(FetchSpdlog)
