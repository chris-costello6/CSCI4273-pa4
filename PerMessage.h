#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <utility>
#include <pthread.h>

#include "Message.h"
#include "ThreadPool.h"
#include "Headers.h"
#include "udpUtils.h"

#define THREAD_COUNT 25

using namespace std;

class PerMessage
{	
public:
	
	PerMessage(int sourcePort, int destPort);
	PerMessage( );
	
	void ethernetSend(int protocol, Message* mesg);
	static void ethernetRecv(void* arg);



private:
	ThreadPool *threads;
	int inUdpPort;
	int destUdpPort;
	void* listenOnSocket(void* arg);
};

PerMessage::PerMessage() 
{
	// Create thread to listen 

}

PerMessage::PerMessage(int sourcePort, int destPort) 
{
	threads = new ThreadPool(25);
	inUdpPort = sourcePort;
	destUdpPort = destPort;
	pthread_t* t;
	int fail = pthread_create(&t, NULL, PerMessage::listenOnSocket, (void*) this);
}

void
PerMessage::ethernetSend(int protocol, Message* mesg)
{

}

void
PerMessage::ethernetRecv(void* arg)
{
	cout << "Got mesg " << arg << endl;
	(Message*) mesg = (Message*) arg;
	(ethHeader*) header = (ethHeader*) mesg->msgStripHder(sizeof(ethHeader));
}

void*
PerMessage::listenOnSocket(void* arg)
{
	PerMessage* pm = (PerMessage*) arg;
	int listeningSock = udpSocket(pm->inUdpPort);
	cout << "Listening on port " << pm->inUdpPort << endl;
	while(true) {
		char* raw = getUdpMesg(listeningSock);
		Message mesg = Message(raw, sizeof(raw));
		pm->threads->dispatchThread(PerMessage::ethernetRecv, (void*)rawMesg);
	}
}


