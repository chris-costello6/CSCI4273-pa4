CC = g++
CCFLAGS = -std=gnu++11 -pthread -g

all: perMessageMake speedTestMake

perMessageMake: PerMessageTest.cc
	$(CC) $(CCFLAGS) -o PerMessageTest PerMessageTest.cc 

speedTestMake: SpeedTest.cc
	$(CC) $(CCFLAGS) -o SpeedTest SpeedTest.cc

clean: 
	rm -rf SpeedTest PerMessageTest *.dSYM
