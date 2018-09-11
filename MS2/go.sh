#!/bin/bash

#SBATCH --job-name=jeadie
#SBATCH --partition=coursework
#SBATCH --time=0:05:00
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=1

module load mpi/openmpi-x86_64

mpirun -n 4 main.exe new_tests/5_True.txt