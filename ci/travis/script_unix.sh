#!/usr/bin/env sh

script_exit_code=0
failed_steps=""
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
        failed_steps="$failed_steps\n    $step_count:\"$desc\""
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

check_result "Build tests" "make" "build_test"
# Run unit tests with verbose output
check_result "Execute tests" "make" "test" "ARGS='-V'"

# Run linters (but don't check output yet)
check_result "Run Linters" "make" "lint"

check_result "Leave Build directory" "cd" ".."
# Check ClangFormat output
check_result "Check formatting" "./ci/travis/check_clang_format.sh"
check_result "Update Site" "./ci/travis/deploy_website.sh"

if [ $script_exit_code -eq 0 ];then
    echo "########################################################################"
    echo "# Exiting Travis Unix Script - Success! "
    echo "########################################################################"
else
    echo "########################################################################"
    echo "# Exiting Travis Unix Script - Error: Failed running step(s): $failed_steps."
    echo "########################################################################"
fi

# Exit with the number of the last step that failed
exit $script_exit_code
