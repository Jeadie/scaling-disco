#!/bin/bash

#SBATCH --job-name=jeadie
#SBATCH --partition=macondo
#SBATCH --time=0:6:00
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=2

module load mpi/openmpi-x86_64
rm log.txt
mpirun -n 4 main.exe false/20.txt 2> log.txt
