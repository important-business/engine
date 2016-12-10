if(NOT TARGET oclint)
    add_custom_target (
        oclint
        )
endif()

if (DEFINED ENV{OCLINT})
    set(OCLINT_BIN "$ENV{OCLINT}")
    message("Using ${OCLINT_BIN} for oclint")
else()
    set(OCLINT_BIN "oclint")
endif()

function(OcLint)
    # Parse Arguments
    set(options OPTIONAL)
    set(oneValueArgs DESTINATION TARGETNAME ENABLE CXX_STANDARD)
    set(multiValueArgs TARGETS FILES INCLUDEDIRS)
    cmake_parse_arguments(OcLint "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    # Set defaults for unprovided arguments
    if(NOT OcLint_ENABLE)
        set(OcLint_ENABLE "warning,performance")
    endif()

    foreach(dir ${OcLint_INCLUDEDIRS})
        set(INCLUDE_ARGS "${INCLUDE_ARGS}" "-I${dir}")
    endforeach()

    add_custom_target(
        oclint_${OcLint_TARGETNAME}
        COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.oclint ${CMAKE_CURRENT_BUILD_DIR}
        COMMAND ${OCLINT_BIN} ${OcLint_FILES} -- ${INCLUDE_ARGS} -std=c++${OcLint_CXX_STANDARD}
    )

add_dependencies(oclint oclint_${OcLint_TARGETNAME})

endfunction(OcLint)
