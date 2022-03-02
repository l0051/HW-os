all: main

signal_handling: signal_handling.o
	g++ signal_handling.o -o signal_handling

signal_handling.o: signal_handling.cpp
	g++ -c signal_handling.cpp -o signal_handling.o

clean:
	rm *.o signal_handling