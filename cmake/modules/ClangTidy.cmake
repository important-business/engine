add_custom_target (
    clangtidy_all
    )

function(ClangTidy)
    # Parse Arguments
    set(options OPTIONAL)
    set(oneValueArgs DESTINATION STYLE TARGETNAME CONFIG)
    set(multiValueArgs TARGETS FILES INCLUDEDIRS)
    cmake_parse_arguments(ClangTidy "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
    message("Including ${ClangTidy_INCLUDEDIRS}")
    # Set defaults for unprovided arguments
    if(NOT ClangTidy_STYLE)
        set(ClangTidy_STYLE "file")
    endif()

    if (DEFINED ENV{CLANG_TIDY})
        set(CLANG_TIDY_BIN "$ENV{CLANG_TIDY}")
        message("Using ${CLANG_TIDY_BIN} for clangtidy")
    else()
        set(CLANG_TIDY_BIN "clang-tidy")
    endif()

    foreach(dir ${ClangTidy_INCLUDEDIRS})
        set(INCLUDE_ARGS "${INCLUDE_ARGS}" "-I${dir}")
    endforeach()

    add_custom_target(
        clangtidy_${ClangTidy_TARGETNAME}
        COMMAND ${CLANG_TIDY_BIN} ${ClangTidy_FILES} -config="${ClangTidy_CONFIG}" -- ${INCLUDE_ARGS} -std=c++14
    )

add_dependencies(clangtidy_all clangtidy_${ClangTidy_TARGETNAME})

endfunction(ClangTidy)
