#!/usr/bin/env bash
pos=50
count=0

while IFS= read -r line; do
    dir="${line:0:1}"
    dist="${line:1}"

    if [[ $dir == "R" ]]; then
        k0=$(( (100 - pos) % 100 ))
        (( k0 == 0 )) && k0=100
        if (( k0 <= dist )); then
            count=$(( count + 1 + (dist - k0) / 100 ))
        fi
        pos=$(( (pos + dist) % 100 ))
    else
        k0=$(( pos % 100 ))
        (( k0 == 0 )) && k0=100
        if (( k0 <= dist )); then
            count=$(( count + 1 + (dist - k0) / 100 ))
        fi
        pos=$(( (pos - dist) % 100 ))
        (( pos < 0 )) && pos=$(( pos + 100 ))
    fi
done < input

echo "$count"
