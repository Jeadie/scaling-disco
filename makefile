src = *.c
obj = $(src:.c=.o)
CC = gcc
FLAGS = -std=gnu99 -Wall -pedantic -g 

SRC=$(wildcard *.c)

all: $(SRC)
	gcc $^ $(FLAGS)  -o a.out
       
%.o : %.c
	$(CC) -c $(FLAGS) $< -o $@

.PHONY: clean
clean: 
	rm *.o 
	make
