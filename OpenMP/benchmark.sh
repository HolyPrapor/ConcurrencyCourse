#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: ./benchmark.sh <filename>"
    exit 1
fi

file=$1

threads=(1 2 4 6 8 10 12 14 16)

for thread in "${threads[@]}"; do
    echo "Running with $thread threads:"
    export OMP_NUM_THREADS=$thread
    /usr/bin/time ./palindrome $file > /dev/null
done


