#!/bin/bash

#BATCH --job-name=jeadie
#SBATCH --partition=coursework
#SBATCH --time=0:05:00
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=1

module load /intel-mpi/5.1.3.181
mpirun -n 4 main.exe
