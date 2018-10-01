#!/bin/bash

for filename in random_true/{1..50}_True.txt; do
    echo "$filename"
     ./main_slow.exe "$filename" 2>> time_slow.log 1>> results.log
done


