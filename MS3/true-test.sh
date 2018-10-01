#!/bin/bash

for filename in random_true/{1..1000}_True.txt; do
    echo "$filename"
     ./a.out "$filename" 2>> time.log 1>> results.log
done


