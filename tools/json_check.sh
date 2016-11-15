#!/bin/bash
failures=0
for f in $(find . -name '*.json' -not -path './build/*' ); do
    echo "Checking JSON file $f"
    ./build/src/tools/jsoncheck -f $f -u
    if [ $? -eq 0 ];then
        echo "Check passed, writing $f in updated format"
    else
        echo "Error encountered in JSON file $f"
        failures=$((failures+1))
    fi
done

exit $failures

