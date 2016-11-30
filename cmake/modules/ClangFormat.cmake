if(NOT TARGET clangformat)
    add_custom_target (
        clangformat
        ALL
        )
endif()

if (DEFINED ENV{CLANG_FORMAT})
    set(CLANG_FORMAT_BIN "$ENV{CLANG_FORMAT}")
    message("Using ${CLANG_FORMAT_BIN} for clangformat")
else()
    set(CLANG_FORMAT_BIN "clang-format")
endif()


function(ClangFormat)
    # Parse Arguments
    set(options OPTIONAL)
    set(oneValueArgs DESTINATION STYLE TARGETNAME CONFIG CXX_STANDARD)
    set(multiValueArgs TARGETS FILES)
    cmake_parse_arguments(ClangFormat "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    # Set defaults for unprovided arguments
    if(NOT ClangFormat_STYLE)
        set(ClangFormat_STYLE "file")
    endif()


    add_custom_target(
        clangformat_${ClangFormat_TARGETNAME}
        COMMAND ${CLANG_FORMAT_BIN} -i ${ClangFormat_FILES} -style=${ClangFormat_STYLE}
    )

    add_dependencies(clangformat clangformat_${ClangFormat_TARGETNAME})

endfunction(ClangFormat)
