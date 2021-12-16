all: main

main: main.o parallel_scelduler.o
	g++ main.o parallel_scelduler.o -o main -pthread

parallel_scelduler.o: parallel_scelduler.cpp parallel_scelduler.h
	g++ -c parallel_scelduler.cpp

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o main