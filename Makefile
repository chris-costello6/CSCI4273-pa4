CC = g++
CCFLAGS = -std=gnu++11 -pthread -g

all: threadPoolMake messageMake

threadPoolMake: ThreadPool.h
	$(CC) $(CCFLAGS) -o ThreadPool ThreadPool.h

messageMake: Message.h
	$(CC) $(CCFLAGS) -o Message Message.h 

clean: 
	rm -f ThreadPool Message
