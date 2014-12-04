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
#include "ourLibs/ThreadPool.h"
// #include "newLibs/threadpool.cpp"
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
	
	void dnsSend(int protocol, Message* mesg);
	static void dnsRecv(Message* mesg);
	void rdpSend(int protocol, Message* mesg);
	static void rdpRecv(Message* mesg);
	void ftpSend(int protocol, Message* mesg);
	static void ftpRecv(Message* mesg);
	void telnetSend(int protocol, Message* mesg);
	static void telnetRecv(Message* mesg);

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
	cout << "My Port: " << inUdpPort << " Dest Port: " << destUdpPort << endl;
	
	pthread_t t;
	int fail = pthread_create(&t, NULL, listenOnSocket, (void*)this);
}

void*
PerMessage::listenOnSocket(void* arg)
{
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
	// Build and Attach a header
	ethHeader eth;
	eth.hlp = protocol;
	for(int i = 0; i< sizeof(eth.otherInfo); i++) eth.otherInfo[i] = 'f';
	eth.length = mesg->msgLen();
	mesg->msgAddHdr((char*)&eth, sizeof(ethHeader));

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
	ethHeader* header = (ethHeader*) mesg->msgStripHdr(sizeof(ethHeader));
	if(header->hlp != IP_ID) {
		fprintf(stderr, "Ethernet encountered Invalid protocol %d\n", header->hlp);
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
	int nextProto = ip->hlp;
	switch(nextProto)
	{
		case TCP_ID:
			tcpRecv(mesg);
			break;
		case UDP_ID:
			udpRecv(mesg);
			break;
		default:
			fprintf(stderr, "IP Encountered Invalid protocol %d\n", nextProto);
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
	switch(tcp->hlp)
	{
		case FTP_ID:
			ftpRecv(mesg);
			break;
		case TELNET_ID:
			telnetRecv(mesg);
			break;
		default:
			fprintf(stderr, "TCP Encountered Invalid protocol %d\n", tcp->hlp);
	}
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
	// char* buff = new char[30];
	// mesg->msgFlat(buff);
	// cout << "UDP LAYER: content=" << buff << " hlp=" << udp->hlp << endl;
	switch(udp->hlp)
	{
		case DNS_ID:
			dnsRecv(mesg);
			break;
		case RDP_ID:
			rdpRecv(mesg);
			break;
		default:
			fprintf(stderr, "UDP Encountered Invalid protocol %d\n", udp->hlp);
	}
}

void
PerMessage::rdpSend(int protocol, Message* mesg)
{
	RDPHeader rdp;
	rdp.hlp = protocol;
	rdp.length = mesg->msgLen();

	mesg->msgAddHdr((char*)&rdp, sizeof(rdp));
	udpSend(RDP_ID, mesg);
}

void
PerMessage::dnsSend(int protocol, Message* mesg)
{
	DNSHeader dns;
	dns.hlp = protocol;
	dns.length = mesg->msgLen();

	mesg->msgAddHdr((char*)&dns, sizeof(dns));
	udpSend(DNS_ID, mesg);
}

void
PerMessage::ftpSend(int protocol, Message* mesg)
{
	FTPHeader ftp;
	ftp.hlp = protocol;
	ftp.length = mesg->msgLen();

	mesg->msgAddHdr((char*)&ftp, sizeof(ftp));
	tcpSend(FTP_ID, mesg);
}

void
PerMessage::telnetSend(int protocol, Message* mesg)
{
	telnetHeader telnet;
	telnet.hlp = protocol;
	telnet.length = mesg->msgLen();

	mesg->msgAddHdr((char*)&telnet, sizeof(telnet));
	tcpSend(TELNET_ID, mesg);
}

void
PerMessage::rdpRecv(Message* mesg)
{
	RDPHeader* rdp = (RDPHeader*) mesg->msgStripHdr(sizeof(RDPHeader));
	char* buff = new char[1024];
	mesg->msgFlat(buff);
	// cout << "RDP MESSAGE: " << buff << endl;
	delete buff;
}

void
PerMessage::dnsRecv(Message* mesg)
{
	DNSHeader* dns = (DNSHeader*) mesg->msgStripHdr(sizeof(DNSHeader));
	char* buff = new char[1024];
	mesg->msgFlat(buff);
	// cout << "DNS MESSAGE: " << buff << endl;
	delete buff;
}

void
PerMessage::ftpRecv(Message* mesg)
{
	FTPHeader* ftp = (FTPHeader*) mesg->msgStripHdr(sizeof(FTPHeader));
	char* buff = new char[1024];
	mesg->msgFlat(buff);
	// cout << "FTP MESSAGE: " << buff << endl;
	delete buff;
}

void
PerMessage::telnetRecv(Message* mesg) 
{
	telnetHeader* telnet = (telnetHeader*) mesg->msgStripHdr(sizeof(telnetHeader));
	char* buff = new char[1024];
	mesg->msgFlat(buff);
	// cout << "TELNET MESSAGE: " << buff << endl;
	delete buff;
}
