CC = g++
FLAGS = -g -c -Wall

all: kry.o
	$(CC) -g kry.o -o kry

kry.o: kry.cpp
	$(CC) $(FLAGS) kry.cpp 

clean:
	rm -f kry.o kry