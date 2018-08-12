#!/bin/bash

for filename in tests/??_True.txt; do
    echo "$filename"
    ./a.out "$filename" 2>> time_T.log 1>> results_T.log
done



for filename in tests/??_False.txt; do
    echo "$filename"
    ./a.out "$filename" 2>> time_F.log 1>> results_F.log
done
