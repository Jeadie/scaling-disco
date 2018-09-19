#!/bin/bash

#SBATCH --job-name=jeadie
#SBATCH --partition=coursework
#SBATCH --time=0:20:00
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=2

module load mpi/openmpi-x86_64
rm log.txt
mpirun -n 8 main.exe new_tests/50_False.txt 2> log.txt
