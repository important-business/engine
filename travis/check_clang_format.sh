#!/usr/bin/env sh
# Note - must be run from the build directory

if [ -z "$CLANG_FORMAT" ]; then
    CLANG_FORMAT="clang-format"
fi

echo "ClangFormat version output: $(${CLANG_FORMAT} --version)"
make clangformat
gitoutput=$(git status -s)
if [ -z "$gitoutput" ]; then
    echo "No changes made by ClangFormat"
    exit 0
else
    echo "ClangFormat found issues with the following files"
    echo "$gitoutput"
    echo "Detailed diff information follows:"
    git --no-pager diff
    echo "Restoring original repository state"
    git reset --hard HEAD
    exit 1
fi

