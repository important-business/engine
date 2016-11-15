#!/usr/bin/env sh
# Note - must be run from the build directory

make json_validate
gitoutput=$(git status -s)
if [ -z "$gitoutput" ]; then
    echo "No changes made by JSON check "
    exit 0
else
    echo "JSON check found issues with the following files"
    echo "$gitoutput"
    echo "Detailed diff information follows:"
    git --no-pager diff
    echo "Restoring original repository state"
    git reset --hard HEAD :/
    exit 1
fi

