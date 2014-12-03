#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <utility>
#include <pthread.h>
#include <unistd.h>

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
	void ipSend(int protocol, Message* mesg);



private:
	ThreadPool *threads;
	int inUdpPort;
	int destUdpPort;
	int mySock;
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
	mySock = udpSocket(inUdpPort);
	cout << "inPort =" << inUdpPort << " destPort=" << destUdpPort << endl;
	threads->dispatch_thread(listenOnSocket, this);
}

void
PerMessage::ethernetSend(int protocol, Message* mesg)
{
	// Attach a header and send it over the network
	ethHeader eth;
	eth.hlp = protocol;
	eth.length = mesg->msgLen();
	
	// cout << "ethHeader: hlp=" << eth.hlp << " other=" << eth.otherInfo
	// << " length=" << eth.length << endl;
	// cout << "size of mesg before=" << mesg->msgLen() << endl;
	// char content[30];
	// mesg->msgFlat(content);
	// cout << "content is " << content << " before sending" << endl;
	

	mesg->msgAddHdr((char*)&eth, sizeof(ethHeader));
	

	cout << "size of mesg after=" << mesg->msgLen() << endl;
	// ethHeader* after = (ethHeader*) mesg->msgStripHdr(sizeof(ethHeader));
	// cout << "Stripped header: hlp=" << after->hlp << " otherinfo=" << after->otherInfo
	// << " length="<< after->length << endl;
	// bzero(content, 30);
	// mesg->msgFlat(content);
	// cout << "Content= " << content << endl;

	// Send message over network
	const char* destHost = "localhost";
	char buffer[1024];
	mesg->msgFlat(buffer);
	cout << "attempting to send " << buffer<< " to " << destHost << " port " << destUdpPort << endl;
	// int sendSock = udpSocket(inUdpPort);
	// sendUdp(mySock, buffer, destHost, destUdpPort);
}

void
PerMessage::ethernetRecv(void* arg)
{
	cout << "ethernetRecv: Got mesg" << endl;
	char* mm = (char*) arg;
	Message mesg(mm, strlen(mm));
	cout << "size of mm=" << sizeof(mm) << " content=" << mm
		 << endl;

	// cout << "size before msgStripHdr=" << mesg.msgLen() << endl;

	ethHeader* header = (ethHeader*) mesg.msgStripHdr(sizeof(ethHeader));

	cout << "size after msgStripHdr=" << mesg.msgLen() << endl;
	cout << "hlp=" << header->hlp << " otherinfo=" << header->otherInfo
	<< " length="<< header->length << endl;
}
void
PerMessage::listenOnSocket(void* arg)
{
	cout << "listen on socket got called" << endl;	
	PerMessage* pm = (PerMessage*) arg;
	cout << "Listening on port " << pm->inUdpPort << endl;
	while(true) {	
		char* raw = getUdpMesg(pm->mySock);
		cout << "got message " << raw << " size=" << strlen(raw)<< endl;
		pm->threads->dispatch_thread(PerMessage::ethernetRecv, (void*)raw);
	}
}


