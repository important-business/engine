# TODO(Keegan): Add option to link statically
function(FetchJsonCpp)
    set(options)
    set(oneValueArgs GITVERSION)
    set(multiValueArgs)
    cmake_parse_arguments(FetchJsonCpp "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
    # Set defaults for unprovided arguments
    if(NOT FetchJsonCpp_GITVERSION)
        set(FetchJsonCpp_GITVERSION "master")
    endif()

    # Enable ExternalProject CMake module
    include(ExternalProject)

    # Download and Install JsonCpp
    # Based off http://www.kaizou.org/2014/11/gtest-cmake/
    # Thanks to David Corvoysier and Henry Schreiner
    # TODO(Keegan): Select version of JsonCpp
    ExternalProject_Add(
        jsoncpp_proj
        GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
        GIT_TAG ${FetchJsonCpp_GITVERSION}
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/jsoncpp
        # Disable install step
        INSTALL_COMMAND ""
    )

    ExternalProject_Get_Property(jsoncpp_proj source_dir binary_dir)

    # Create an interface library for tests to link against
    add_library(jsoncpp INTERFACE)
    add_dependencies(jsoncpp jsoncpp_proj)
    target_link_libraries(
        jsoncpp
        INTERFACE
        "${binary_dir}/src/lib_json/${CMAKE_SHARED_LIBRARY_PREFIX}jsoncpp${CMAKE_STATIC_LIBRARY_SUFFIX}"
    )
    # TODO(Keegan): Is this the best way to export the include directory?
    set(JSONCPP_INCLUDE_DIR "${source_dir}/include" "${binary_dir}/include" PARENT_SCOPE)
endfunction(FetchJsonCpp)
