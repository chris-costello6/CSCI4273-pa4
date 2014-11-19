CC = g++
CCFLAGS = -std=gnu++11 -pthread -g

all: listenHostMake sendHostMake

listenHostMake: listenHost.cc
	$(CC) $(CCFLAGS) -o listenhost listenHost.cc

sendHostMake: sendHost.cc
	$(CC) $(CCFLAGS) -o sendhost sendHost.cc 

clean: 
	rm -rf sendhost listenhost *.dSYM
