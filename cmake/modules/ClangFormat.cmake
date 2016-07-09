function(ClangFormat)
    # Parse Arguments
    set(options OPTIONAL)
    set(oneValueArgs DESTINATION STYLE)
    set(multiValueArgs TARGETS CLANGFORMAT_FILES)
    cmake_parse_arguments(ClangFormat "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    # Set defaults for unprovided arguments
    if(NOT ClangFormat_STYLE)
        set(ClangFormat_STYLE "file")
    endif()

    add_custom_target(
            clangformat
            COMMAND clang-format -i ${ClangFormat_CLANGFORMAT_FILES} -style=${ClangFormat_STYLE}
    )

endfunction(ClangFormat)
