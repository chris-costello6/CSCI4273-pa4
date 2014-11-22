CC = g++
CCFLAGS = -std=gnu++11 -pthread -g

all: threadPoolMake messageMake sendHostMake listenHostMake

listenHostMake: listenHost.cc
	$(CC) $(CCFLAGS) -o listenhost listenHost.cc

sendHostMake: sendHost.cc
	$(CC) $(CCFLAGS) -o sendhost sendHost.cc 

sendHostMake: sendHost.cc
	$(CC) $(CCFLAGS) -o sendHost sendHost.cc

listenHostMake: listenHost.cc
	$(CC) $(CCFLAGS) -o listenHost listenHost.cc

clean: 
	rm -rf ThreadPool Message sendHost listenHost *.dSYM
