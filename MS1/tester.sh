#!/bin/bash

for filename in new_tests/??_False.txt; do
    echo "$filename"
     ./a.out "$filename" 2>> time_F.log 1>> results_F.log 
     ./a.out "$filename" 2>> time_F.log 1>> results_F.log
     ./a.out "$filename" 2>> time_F.log 1>> results_F.log
done

for filename in new_tests/??_True.txt; do
    echo "$filename"
    ./a.out "$filename" 2>> time_T.log 1>> results_T.log 
    ./a.out "$filename" 2>> time_T.log 1>> results_T.log 
    ./a.out "$filename" 2>> time_T.log 1>> results_T.log 
done



