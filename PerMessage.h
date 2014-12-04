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
	static void ipRecv(Message* mesg);
	void tcpSend(int protocol, Message* mesg);
	static void tcpRecv(Message* mesg);
	void udpSend(int protocol, Message* mesg);
	static void udpRecv(Message* mesg);


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

void*
PerMessage::listenOnSocket(void* arg)
{
	// cout << "listen on socket got called" << endl;	
	PerMessage* pm = (PerMessage*) arg;
	cout << "Listening on port " << pm->inUdpPort << endl;
	while(true) {
		char* buffer = new char[BUFSIZE]; // Move this outside the loop and you're fucked.
		memset(buffer, 0, BUFSIZE);	
		int n = getUdpMesg(pm->mySock, buffer, BUFSIZE);
		Message* m = new Message(buffer, n); //Only read in num bytes read
		pm->threads->dispatch_thread(PerMessage::ethernetRecv, (void*) m);
	}
}

void
PerMessage::ethernetSend(int protocol, Message* mesg)
{
	// Attach a header
	ethHeader eth;
	eth.hlp = protocol;
	for(int i = 0; i< sizeof(eth.otherInfo); i++) eth.otherInfo[i] = 'f';
	eth.length = mesg->msgLen();
	
	// cout << "size of mesg before=" << mesg->msgLen() << endl;

	mesg->msgAddHdr((char*)&eth, sizeof(ethHeader));
	
	// cout << "size of mesg after adding header=" << mesg->msgLen() << endl;

	// Send message over network
	const char* destHost = "localhost";
	char* buffer = new char[1024];
	mesg->msgFlat(buffer);
	sendUdp(mySock, buffer, mesg->msgLen(), destHost, destUdpPort);
}

void
PerMessage::ethernetRecv(void* arg)
{

	Message* mesg = (Message*) arg;
	// char* content = new char[30];
	// mesg->msgFlat(content);
	// cout << "ethernetRecv Message content" << endl;
	// cout << "content=" << content <<" size before msgStripHdr=" << mesg->msgLen() << endl;

	ethHeader* header = (ethHeader*) mesg->msgStripHdr(sizeof(ethHeader));
	// cout << "size after msgStripHdr=" << mesg->msgLen() << endl;
	// cout << "hlp=" << header->hlp << " otherinfo=" << header->otherInfo
	// << " length="<< header->length  << endl;
	if(header->hlp != IP_ID) {
		fprintf(stderr, "Invalid protocol %d\n", header->hlp);
		exit(1);
	}
	//Send to IP
	ipRecv(mesg);
}
void
PerMessage::ipSend(int protocol, Message* mesg)
{
	IPHeader ip;
	ip.hlp = protocol;
	ip.length = mesg->msgLen();

	mesg->msgAddHdr((char*)&ip, sizeof(IPHeader));

	ethernetSend(IP_ID, mesg);
}
void
PerMessage::ipRecv(Message* mesg) 
{
	IPHeader* ip = (IPHeader*) mesg->msgStripHdr(sizeof(IPHeader));
	// char* buff = new char[40];
	// mesg->msgFlat(buff);
	// cout << "IP LAYER: content=" << buff << " hlp=" << ip->hlp << endl;
	int nextProto = ip->hlp;
	switch(nextProto)
	{
		case TCP_ID:
			cout << "IP Layer Sending to TCP" << endl;
			tcpRecv(mesg);
			break;
		case UDP_ID:
			udpRecv(mesg);
			break;
		default:
			fprintf(stderr, "Invalid protocol %d\n", nextProto);
	}

}
void
PerMessage::tcpSend(int protocol, Message* mesg)
{
	TCPHeader tcp;
	tcp.hlp = protocol;
	tcp.length = mesg->msgLen();

	mesg->msgAddHdr((char*)&tcp, sizeof(TCPHeader));
	ipSend(TCP_ID, mesg);
}

void
PerMessage::tcpRecv(Message* mesg)
{
	TCPHeader* tcp = (TCPHeader*) mesg->msgStripHdr(sizeof(TCPHeader));
	char* buff = new char[40];
	mesg->msgFlat(buff);
	cout << "TCP LAYER: content=" << buff << " hlp=" << tcp->hlp << endl;
	// switch(tcp->hlp)
	// {
	// 	case FTP_ID:
	// 		//ftprecv
	// 		break;
	// 	case TELNET_ID:
	// 		//telnetRecv
	// 		break;
	// 	default:
	// 		fprintf(stderr, "Invalid protocol %d\n", tcp->hlp);
	// }
}

void
PerMessage::udpSend(int protocol, Message* mesg)
{
	UDPHeader udp;
	udp.hlp = protocol;
	udp.length = mesg->msgLen();

	mesg->msgAddHdr((char*)&udp, sizeof(UDPHeader));
	ipSend(UDP_ID, mesg);
}

void
PerMessage::udpRecv(Message* mesg) 
{
	UDPHeader* udp = (UDPHeader*) mesg->msgStripHdr(sizeof(UDPHeader));
	char* buff = new char[30];
	mesg->msgFlat(buff);
	cout << "UDP LAYER: content=" << buff << " hlp=" << udp->hlp << endl;
	// switch(udp->hlp)
	// {
	// 	case DNS_ID:
	// 		//dnsRecv
	// 		break;
	// 	case RDP_ID:
	// 		//rdpRecv
	// 		break;
	// 	default
	// 		fprintf(stderr, "Invalid protocol %d\n", udp->hlp);
	// }
}

