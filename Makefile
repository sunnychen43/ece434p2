COMPILER = gcc
FLAGS = -Wall -g 


kill:
	killall -9 -v -e p1.out
run: 
	./p1.out

signal:
	killall -s $(s) p1.out

all: p1.out
p1: p1.out 


p1.o: p1.c p1.h
	$(COMPILER) $(FLAGS) -c p1.c

p1.out: p1.o
	$(COMPILER) $(FLAGS) -o p1.out p1.o -lpthread


clean:
	rm -f *.o *.out