# scaling-disco
COSC3500: High Performance Computing 

##Usage 

There is a makefile included, that depends on the mpicc compiler. To add it: 
`module load mpi/openmpi-x86_64`

Then `make`the program. 

Finally a sample batch script is found in go.sh. To change the testing [GRAPH FILE] is done by changing it 
in line 11:
`mpirun -n 2 main.exe new_tests/60.txt 2> log.txt`
 
