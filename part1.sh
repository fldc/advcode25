#!/usr/bin/env bash

pos=50
count=0

while read -r line; do
    dir="${line:0:1}"
    dist="${line:1}"

    if [[ $dir == "R" ]]; then
        pos=$(( (pos + dist) % 100 ))
    else
        pos=$(( (pos - dist) % 100 ))
        (( pos < 0 )) && pos=$(( pos + 100 ))
    fi

    (( pos == 0 )) && ((count++))
done < input

echo "$count"
