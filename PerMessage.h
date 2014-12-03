#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <utility>
#include <pthread.h>

#include "Message.h"
#include "threadPool.h"
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
	static void listenOnSocket(void* arg);

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
	threads->dispatch_thread(listenOnSocket, this);
}

void
PerMessage::ethernetSend(int protocol, Message* mesg)
{

}

void
PerMessage::ethernetRecv(void* arg)
{
	// cout << "ethernetRecv: Got mesg " << arg << endl;
	char* mm = (char*) arg;
	Message mesg(mm, strlen(mm));
	cout << "size of mm=" << sizeof(mm) << " content=" << mm
		<< " size of ethHeader=" << sizeof(ethHeader) << endl;

	cout << "size before msgStripHdr=" << mesg.msgLen() << endl;

	ethHeader* header = (ethHeader*) mesg.msgStripHdr(sizeof(ethHeader));
	cout << "size after msgStripHdr=" << mesg.msgLen() << endl;
	// cout << "hlp=" << header->hlp << " otherinfo=" << header->otherInfo
	// << " length="<< header->length << endl;
}
void
PerMessage::listenOnSocket(void* arg)
{
	cout << "listen on socket got called" << endl;	
	PerMessage* pm = (PerMessage*) arg;
	int listeningSock = udpSocket(pm->inUdpPort);
	cout << "Listening on port " << pm->inUdpPort << endl;
	while(true) {	
		char* raw = getUdpMesg(listeningSock);
		// cout << "got message " << raw << " size=" << strlen(raw)<< endl;
		pm->threads->dispatch_thread(PerMessage::ethernetRecv, (void*)raw);
	}
}


