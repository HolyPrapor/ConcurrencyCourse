#!/bin/bash

processes=(1 2 4 6 8)

for count in "${processes[@]}"; do
    echo "Running with $count processes:"
    /usr/bin/time  mpirun -np $count ./game_of_life
done

