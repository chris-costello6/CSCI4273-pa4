CC=g++
FLAGS=-g -std=gnu++11 -Wall -Wextra
LIBS=-lpthread

all: driver1 driver2 driver3 threadpool.o eventscheduler.o

driver1: driver1.cc threadpool.o
	$(CC) $(FLAGS) -o $@ $^ $(LIBS)

driver2: driver2.cc eventscheduler.o threadpool.o
	$(CC) $(FLAGS) -o $@ $^ $(LIBS)

driver3: driver3.cc message.o
	$(CC) $(FLAGS) -o $@ $^ $(LIBS)

threadpool.o: threadpool.cpp
	$(CC) $(FLAGS) -o $@ -c $^

eventscheduler.o: eventscheduler.cpp
	$(CC) $(FLAGS) -o $@ -c $^

message.o: message.cpp
	$(CC) $(FLAGS) -o $@ -c $^

clean:
	rm -f threadpool.o eventscheduler.o driver1 driver2 driver3
