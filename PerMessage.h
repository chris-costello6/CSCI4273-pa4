#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <utility>
#include <pthread.h>
#include <unistd.h>

#include "ourLibs/Message.h"
#include "newLibs/threadpool.cpp"
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
	static void* listenOnSocket(void* arg);

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
	
	pthread_t t;
	int fail = pthread_create(&t, NULL, listenOnSocket, (void*)this);
}

void
PerMessage::ethernetSend(int protocol, Message* mesg)
{
	// Attach a header and send it over the network
	ethHeader eth;
	eth.hlp = protocol;
	for(int i = 0; i< sizeof(eth.otherInfo); i++) eth.otherInfo[i] = 'f';
	eth.length = mesg->msgLen();
	
	// cout << "ethHeader: hlp=" << eth.hlp << " other=" << eth.otherInfo
	// << " length=" << eth.length << endl;
	// char content[30];
	// mesg->msgFlat(content);
	// cout << "content is " << content << " before sending" << endl;
	
	cout << "size of mesg before=" << mesg->msgLen() << endl;

	mesg->msgAddHdr((char*)&eth, sizeof(ethHeader));
	

	cout << "size of mesg after adding header=" << mesg->msgLen() << endl;

	// ethHeader* after = (ethHeader*) mesg->msgStripHdr(sizeof(ethHeader));
	// cout << "Stripped header: hlp=" << after->hlp << " otherinfo=" << after->otherInfo
	// << " length="<< after->length << endl;

	// bzero(content, 30);
	// mesg->msgFlat(content);
	// cout << "Content =" << content << endl;

	// Send message over network
	const char* destHost = "localhost";
	char* buffer = new char[1024];
	mesg->msgFlat(buffer);
	// for(int i = 0; i < mesg->msgLen(); i++) buffer[i] = 't';
		// printf("%c\n", buffer[i]);
	// cout << "attempting to send " << buffer<< " to " << destHost
	//  << " port " << destUdpPort << endl;
	// int sendSock = udpSocket(inUdpPort);
	sendUdp(mySock, buffer, mesg->msgLen(), destHost, destUdpPort);
}

void
PerMessage::ethernetRecv(void* arg)
{
	// cout << "ethernetRecv: Got mesg" << endl;
	// char* xx= (char*) arg;
	// for(int i = 0; i < 30; i++) printf("%c", xx[i]);
	// cout << "content =" << xx << endl;
	Message* mesg = (Message*) arg;
	// printf("%x\n", mesg);
	char* content = new char[30];
	mesg->msgFlat(content);
	cout << "ethernetRecv Message content" << endl;
	for(int i = 0; i < 30; i++) printf("%c", content[i]);
	cout << endl;
	// cout << "content=" << content <<" size before msgStripHdr=" << mesg->msgLen() << endl;

	ethHeader* header = (ethHeader*) mesg->msgStripHdr(sizeof(ethHeader));
	cout << "size after msgStripHdr=" << mesg->msgLen() << endl;
	cout << "hlp=" << header->hlp << " otherinfo=" << header->otherInfo
	<< " length="<< header->length  << endl;

	//Send to IP
}
void*
PerMessage::listenOnSocket(void* arg)
{
	struct sockaddr_in theirAddr;
	socklen_t len;
	cout << "listen on socket got called" << endl;	
	PerMessage* pm = (PerMessage*) arg;
	cout << "Listening on port " << pm->inUdpPort << endl;
	char* buffer = new char[BUFSIZE];
	// char* xx = new char[15];
	while(true) {
		memset(buffer, 0, BUFSIZE);	
		// int n = getUdpMesg(pm->mySock, buffer, BUFSIZE);
		int n = recvfrom(pm->mySock, buffer, BUFSIZE, 0, 
		    		(struct sockaddr *)&theirAddr, &len);
		Message* m = new Message(buffer, n); //Only read in num bytes read
		//printf("%x\n", m);
		char mm[n];// = new char[n];
		m->msgFlat(mm);
		// cout << "n=" << n << " mesgLen=" << m->msgLen() << endl;
		cout << "Before dispatch_thread" << endl;
		for(int i = 0; i < 30; i++) printf("%c", mm[i]);
		cout << endl;
		// cout << "passing in new char* instead of message." << endl;
		// for (int i = 0; i < 15; ++i) xx[i] = 'x';
		pm->threads->dispatch_thread(PerMessage::ethernetRecv, (void*) m);
	}
}


