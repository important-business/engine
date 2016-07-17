#!/usr/bin/env sh
# Note - must be run from the build directory
make clangformat
gitoutput=$(git status -s)
if [[ -z $gitoutput ]]; then
    echo "No changes made by ClangFormat"
    exit 0
else
    echo "ClangFormat found issues with the following files"
    echo "$gitoutput"
    exit 1
fi

