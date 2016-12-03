function(FetchGoogleTest)
    set(options)
    set(oneValueArgs GITVERSION BUILD_SHARED)
    set(multiValueArgs)
    cmake_parse_arguments(FetchGoogleTest "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    # Set defaults for unprovided arguments
    if(NOT FetchGoogleTest_GITVERSION)
        set(FetchGoogleTest_GITVERSION "master")
    endif()

    # Required for google test
    find_package(Threads REQUIRED)

    # Enable ExternalProject CMake module
    include(ExternalProject)

    # Download and install GoogleTest
    # Based off http://www.kaizou.org/2014/11/gtest-cmake/
    # Thanks to David Corvoysier and Henry Schreiner
    # TODO: Update Gtest?
    ExternalProject_Add(
        gtest_proj
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG ${FetchGoogleTest_GITVERSION}
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
        # Disable install step
        INSTALL_COMMAND ""
    )

    ExternalProject_Get_Property(gtest_proj source_dir binary_dir)

    # Create an interface library for tests to link against
    add_library(gtest INTERFACE)
    add_dependencies(gtest gtest_proj)
    target_link_libraries(
        gtest
        INTERFACE
        Threads::Threads
        "${binary_dir}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}"
    )
    set(GTEST_INCLUDE_DIR "${source_dir}/include" "${binary_dir}/include" PARENT_SCOPE)
endfunction(FetchGoogleTest)
