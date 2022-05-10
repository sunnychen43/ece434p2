COMPILER = gcc
FLAGS = -Wall -g


kill:
	killall -9 -v -e p1.out killall -9 -v -e p2.out killall -9 -v -e p3.out killall -9 -v -e p4.out
run: 
	./p1.out 10 10 10 10

signal:
	killall -s $(s) p1.out

all: p1.out p2.out p3.out p4.out
p1: p1.out 
p2: p2.out
p3: p3.out
p4: p4.out


p1.o: p1.c p1.h
	$(COMPILER) $(FLAGS) -c p1.c

p1.out: p1.o
	$(COMPILER) $(FLAGS) -o p1.out p1.o -pthread

p2.o: p2.c p2.h
	$(COMPILER) $(FLAGS) -c p2.c

p2.out: p2.o
	$(COMPILER) $(FLAGS) -o p2.out p2.o -pthread -lm

p3.o: p3.c p3.h
	$(COMPILER) $(FLAGS) -c p3.c

p3.out: p3.o
	$(COMPILER) $(FLAGS) -o p3.out p3.o -pthread -lm

p4.o: p4.c p4.h
	$(COMPILER) $(FLAGS) -c p4.c

p4.out: p4.o
	$(COMPILER) $(FLAGS) -o p4.out p4.o -pthread

clean:
	rm -f *.o *.out *.txt