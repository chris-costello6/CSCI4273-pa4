CC = g++
CCFLAGS = -std=gnu++11 -O3 -pthread -g

all: perMessageMake speedTestMake perProtocolMake

perMessageMake: PerMessageTest.cc
	$(CC) $(CCFLAGS) -o PerMessageTest PerMessageTest.cc 

speedTestMake: SpeedTest.cc
	$(CC) $(CCFLAGS) -o SpeedTest SpeedTest.cc

# perProtocolMake: PerProtocol_test.cpp
# 	$(CC) $(CCFLAGS) -o PerProtocol_test PerProtocol_test.cpp

clean: 
	rm -rf SpeedTest PerMessageTest *.dSYM
