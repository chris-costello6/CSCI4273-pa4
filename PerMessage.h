#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <utility>

#include "Message.h"
#include "ThreadPool.h"
#include "Headers.h"
#include "udpUtils.h"


using namespace std;

class PerMessage
{	

public:
	
	PerMessage( );

	void ethernetSend(int protocol, Message* mesg);
	static void ethernetRecv(Message* mesg);



private:
	ThreadPool threads(25);
	int inUdpPort;
	int destUdpPort;
	void* listenOnSocket(void* arg);
};