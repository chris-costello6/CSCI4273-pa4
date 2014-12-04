#include <stdio.h>
#include "PerMessage.h"
// #include "PerProtocol.h"
#define USAGE "./SpeedTest"
#define SEND "send"
#define LISTEN "listen"
#define ETHTEST "ethTest"
#define port1 44444
#define port2 45555
#define port3 46666
#define port4 47777
#define SIZE  100
#define SLEEP 12
void* ftp(void* arg);
void* telnet(void* arg);
void* dns(void* arg);
void* rdp(void* arg);

char* text = new char[SIZE];

int main() 
{
	for (int i = 0; i < SIZE; ++i) text[i] = 't';
	pthread_t t[4];
	PerMessage* listen = new PerMessage(port1, port2);
	PerMessage* send = new PerMessage(port2, port1);

	pthread_create(&t[0], NULL, ftp, (void*)send);
	pthread_create(&t[1], NULL, telnet, (void*)send);
	pthread_create(&t[2], NULL, dns, (void*)send);
	pthread_create(&t[3], NULL, rdp, (void*)send);
	

	pthread_join(t[0], NULL);
	pthread_join(t[1], NULL);
	pthread_join(t[2], NULL);
	pthread_join(t[3], NULL);

	return 0;
}

void* ftp(void* arg)
{
	PerMessage* pm = (PerMessage*) arg;
	
	cout << "attempting to send text" << endl;
	for (int i = 0; i < SIZE; ++i)
	{
		Message* mesg = new Message(text, SIZE);
		pm->ftpSend(FTP_ID, mesg);
		usleep(SLEEP);
	}	
	return NULL;
}

void* telnet(void* arg)
{
	PerMessage* pm = (PerMessage*) arg;
	for (int i = 0; i < SIZE; ++i)
	{
		Message* mesg = new Message(text, SIZE);
		pm->telnetSend(TELNET_ID, mesg);
		usleep(SLEEP);
	}
	return NULL;
}

void* dns(void* arg)
{
	PerMessage* pm = (PerMessage*) arg;
	for (int i = 0; i < SIZE; ++i)
	{
		Message* mesg = new Message(text, SIZE);
		pm->dnsSend(DNS_ID, mesg);
		usleep(SLEEP);
	}
	return NULL;
}

void* rdp(void* arg)
{
	PerMessage* pm = (PerMessage*) arg;
	for (int i = 0; i < SIZE; ++i)
	{
		Message* mesg = new Message(text, SIZE);
		pm->rdpSend(RDP_ID, mesg);
		usleep(SLEEP);
	}
	return NULL;
}