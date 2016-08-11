#!/usr/bin/env sh

script_exit_code=0
step_count=1

check_result(){
    desc=$1
    shift
    echo "[TravisScript] Step $step_count: Executing step \"$desc\" command \"$@\""
    "$@"
    exit_code=$?
    if [ $exit_code -ne 0 ];then
        echo "[TravisScript] Step $step_count: Error running step \"$desc\" command \"$@\""
        script_exit_code=$step_count
    else
        echo "[TravisScript] Step $step_count: Success running \"$desc\""
    fi
    step_count=$(($step_count+1))
}

echo "########################################################################"
echo "# Entering Travix Unix Script"
echo "########################################################################"
check_result "Enter Build directory" "cd" "build"
check_result "Generate Makefiles" "cmake" ".."
check_result "Build engine" "make" "engine"

# Run unit tests with verbose output
check_result "Execute tests" "make" "test" "ARGS='-V'"

# Run Clang-Tidy (but don't check output yet)
check_result "Run Clang-Tidy" "make" "clangtidy_all"

# Check ClangFormat output
check_result "Check formatting" "../travis/check_clang_format.sh"

if [ $exit_code -eq 0 ];then
    echo "########################################################################"
    echo "# Exiting Travis Unix Script - Success! "
    echo "########################################################################"
else
    echo "########################################################################"
    echo "# Exiting Travis Unix Script - Error: Failed running step $script_exit_code."
    echo "########################################################################"
fi

# Exit with the number of the last step that failed
exit $script_exit_code
