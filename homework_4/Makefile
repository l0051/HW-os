all: main

main: main.o matrix.o
	g++ main.o matrix.o -o main -pthread

matrix.o: matrix.cpp matrix.h
	g++ -c matrix.cpp

main.o: main.cpp 
	g++ -c main.cpp

clean:
	rm *.o main