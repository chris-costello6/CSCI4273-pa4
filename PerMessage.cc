#include "PerMessage.h"
#include "udpUtils.h"

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
	int listeningSock = udpSocket(pm->inUdpPort);
	while(true) {
		char* raw = getUdpMesg(listeningSock);
		Message mesg = Message(raw, sizeof(raw))
		pm->threads->dispatchThread(PerMessage::ethernetRecv, (void*)rawMesg);
	}
}


