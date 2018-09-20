#!/bin/bash

#SBATCH --job-name=jeadie
#SBATCH --partition=macondo
#SBATCH --time=0:10:00
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=2

module load mpi/openmpi-x86_64
rm log.txt
mpirun -n 8 main.exe new_tests/55.txt 2> log.txt
