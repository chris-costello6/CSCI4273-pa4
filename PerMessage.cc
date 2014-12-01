#include "PerMessage.h"
#include "udpUtils.cc"
PerMessage::PerMessage() 
{
	// Create thread to listen 

}

PerMessage::ethernetSend(int protocol, Message* mesg)
{

}

PerMessage::ethernetRecv(Message* mesg)
{

}

PerMessage::listenOnSocket(void* arg)
{
	PerMessage pm = (PerMessage*) arg;

}
