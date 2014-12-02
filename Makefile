CC = g++
CCFLAGS = -std=gnu++11 -pthread -g

all: perMessageMake

perMessageMake: PerMessageTest.cc
	$(CC) $(CCFLAGS) -o PerMessageTest PerMessageTest.cc 

clean: 
	rm -rf ThreadPool Message PerMessageTest *.dSYM
