all: main

main: main.o parallel_scheduler.o
	g++ main.o parallel_scheduler.o -o main -pthread

parallel_scheduler.o: parallel_scheduler.cpp parallel_scheduler.h
	g++ -c parallel_scheduler.cpp

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o main