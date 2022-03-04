all: producer consumer

produser: produser.o
	g++ produser.o -o produser

consumer: consumer.o
	g++ consumer.o -o consumer

produser.o: produser.cpp
	g++ -c produser.cpp

consumer.o: consumer.cpp
	g++ -c consumer.cpp

clean:
	rm *.o producer consumer
