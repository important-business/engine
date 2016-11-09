#!/bin/bash
failures=0
for f in $(find . -name '*.json' -not -path './build/*' ); do
    echo "Checking JSON file $f"
    fixed=$(cat $f | python -m json.tool)
    if [ $? -eq 0 ];then
        echo "Check passed, writing $f in updated format"
        echo "$fixed" > $f
    else
        echo "Error encountered in JSON file $f"
        failures=$((failures+1))
    fi
done

exit $failures

