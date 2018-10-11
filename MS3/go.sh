#!/bin/bash

###nodes=$nodes
###ntasks_per_node= $ntasks_per_node; 
###time_limit= $time_limit
###
#######SBATCH --job-name=jeadie
####SBATCH --partition=macondo
####SBATCH --time=$time_limit
####SBATCH --nodes=$nodes
####SBATCH --ntasks-per-node=$ntasks_per_node
###
module load mpi/openmpi-x86_64

echo $test_file
mpirun  main.exe $test_file 2> log.txt 
