if(NOT TARGET clangtidy)
    add_custom_target (
        clangtidy
        )
endif()

if (DEFINED ENV{CLANG_TIDY})
    set(CLANG_TIDY_BIN "$ENV{CLANG_TIDY}")
    message("Using ${CLANG_TIDY_BIN} for clangtidy")
else()
    set(CLANG_TIDY_BIN "clang-tidy")
endif()

function(ClangTidy)
    # Parse Arguments
    set(options OPTIONAL)
    set(oneValueArgs DESTINATION STYLE TARGETNAME CONFIG CXX_STANDARD)
    set(multiValueArgs TARGETS FILES INCLUDEDIRS)
    cmake_parse_arguments(ClangTidy "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    # Set defaults for unprovided arguments
    if(NOT ClangTidy_STYLE)
        set(ClangTidy_STYLE "file")
    endif()

    foreach(dir ${ClangTidy_INCLUDEDIRS})
        set(INCLUDE_ARGS "${INCLUDE_ARGS}" "-I${dir}")
    endforeach()

    add_custom_target(
        clangtidy_${ClangTidy_TARGETNAME}
        COMMAND ${CLANG_TIDY_BIN} ${ClangTidy_FILES} -config="${ClangTidy_CONFIG}" -- ${INCLUDE_ARGS} -std=c++${ClangTidy_CXX_STANDARD}
    )

    add_dependencies(clangtidy clangtidy_${ClangTidy_TARGETNAME})

endfunction(ClangTidy)
