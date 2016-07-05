function(ClangFormat)
    set(options OPTIONAL FAST)
    set(oneValueArgs DESTINATION RENAME)
    set(multiValueArgs TARGETS CLANGFORMAT_FILES)
    cmake_parse_arguments(ClangFormat "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    add_custom_target(
            clangformat
            COMMAND /usr/bin/clang-format -i ${ClangFormat_CLANGFORMAT_FILES}
    )

endfunction(ClangFormat)
