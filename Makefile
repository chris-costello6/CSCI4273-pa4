CC = g++
CCFLAGS = -std=gnu++11 -pthread -g

all: threadPoolMake messageMake sendHostMake listenHostMake

threadPoolMake: ThreadPool.h
	$(CC) $(CCFLAGS) -o ThreadPool ThreadPool.h

messageMake: Message.h
	$(CC) $(CCFLAGS) -o Message Message.h 

sendHostMake: sendHost.cc
	$(CC) $(CCFLAGS) -o sendHost sendHost.cc

listenHostMake: listenHost.cc
	$(CC) $(CCFLAGS) -o listenHost listenHost.cc

clean: 
	rm -f ThreadPool Message sendHost listenHost
