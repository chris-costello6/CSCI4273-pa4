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

using namespace std;

class PerMessage
{	

public:
	
	PerMessage( );

	void ethernetSend(int protocol, Message* mesg);
	static void ethernetRecv(Message* mesg);



private:
	ThreadPool tp(25);
	int inUDPSock;
	int outUDPSock;
	void* listenOnSocket(void* arg);
};