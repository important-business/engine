if(NOT TARGET cppcheck)
    add_custom_target (
        cppcheck
        )
endif()

if (DEFINED ENV{CPPCHECK})
    set(CPPCHECK_BIN "$ENV{CPPCHECK}")
    message("Using ${CPPCHECK_BIN} for cppcheck")
else()
    set(CPPCHECK_BIN "cppcheck")
endif()

function(CppCheck)
    # Parse Arguments
    set(options OPTIONAL)
    set(oneValueArgs DESTINATION TARGETNAME ENABLE)
    set(multiValueArgs TARGETS FILES INCLUDEDIRS)
    cmake_parse_arguments(CppCheck "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    # Set defaults for unprovided arguments
    if(NOT CppCheck_ENABLE)
        set(CppCheck_ENABLE "all")
    endif()

    foreach(dir ${CppCheck_INCLUDEDIRS})
        set(INCLUDE_ARGS "${INCLUDE_ARGS}" "-I${dir}")
    endforeach()

    add_custom_target(
        cppcheck_${CppCheck_TARGETNAME}
        COMMAND ${CPPCHECK_BIN} ${CppCheck_FILES} ${INCLUDE_ARGS} --enable=${CppCheck_ENABLE}
    )

add_dependencies(cppcheck cppcheck_${CppCheck_TARGETNAME})

endfunction(CppCheck)
