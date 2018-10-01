src = *.c
obj = $(src:.c=.o)
CC = mpicc
FLAGS = -g 

SRC=$(wildcard *.c)

all: $(SRC)
	$(CC) $^ $(FLAGS) -O3 -o main.exe
	$(CC) $^ $(FLAGS) -O0 -pg -o main_slow.exe
       
%.o : %.c
	$(CC) -c $(FLAGS) $< -o $@

.PHONY: clean
clean: 
	rm *.o 
	make
