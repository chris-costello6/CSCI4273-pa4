#include <stdio.h>
#include <sys/time.h>
#include "PerMessage.h"
#include "PerProtocol.h"


#define USAGE "./SpeedTest"
#define SEND "send"
#define LISTEN "listen"
#define ETHTEST "ethTest"
#define port1 44444
#define port2 45555
#define port3 "46666"
#define port4 "47777"
#define SIZE  100
#define SLEEP 1
#define SLEEEP 600


void* perMessageFTP(void* arg);
void* perMessageTelnet(void* arg);
void* perMessageDNS(void* arg);
void* perMessageRDP(void* arg);

void* perProtocolFTP(void* arg);
void* perProtocolTelnet(void* arg);
void* perProtocolDNS(void* arg);
void* perProtocolRDP(void* arg);

char* text = new char[SIZE];

extern int mCounter;

int main() 
{
	mCounter = 0;

	for (int i = 0; i < SIZE; ++i) text[i] = 't';
	
	// PerMessage Tests -------------------------------------------------------
	timeval start, end;
	pthread_t t[4];
	int micros = 0;

	


	// PerProtocol Tests ------------------------------------------------------
	PerProtocol* perProtocolListen = new PerProtocol(port3, port4);
	PerProtocol* perProtocolSend = new PerProtocol(port4, port3);
	
	gettimeofday(&start, NULL);

	pthread_create(&t[0], NULL, perProtocolFTP, (void*)perProtocolSend);
	pthread_create(&t[1], NULL, perProtocolTelnet, (void*)perProtocolSend);
	pthread_create(&t[2], NULL, perProtocolDNS, (void*)perProtocolSend);
	pthread_create(&t[3], NULL, perProtocolRDP, (void*)perProtocolSend);

	for (int i = 0; i < 4; i++)
		pthread_join(t[i], NULL);

	while(1)
	{
		printf("mCounter: %d\n", mCounter);
		if(mCounter >= 400)
			break;
	}

	gettimeofday(&end, NULL);

	micros = end.tv_usec - start.tv_usec;
	cout << "PerProtocol Total Time: " << micros << " microseconds" << endl;


	// PerMessage Tests -------------------------------------------------------
	
	
	
	PerMessage* perMessageListen = new PerMessage(port1, port2);
	PerMessage* perMessageSend = new PerMessage(port2, port1);
	
	gettimeofday(&start, NULL);
	
	pthread_create(&t[0], NULL, perMessageFTP, (void*)perMessageSend);
	pthread_create(&t[1], NULL, perMessageTelnet, (void*)perMessageSend);
	pthread_create(&t[2], NULL, perMessageDNS, (void*)perMessageSend);
	pthread_create(&t[3], NULL, perMessageRDP, (void*)perMessageSend);	

	for (int i = 0; i < 4; i++)
		pthread_join(t[i], NULL);

	gettimeofday(&end, NULL);

	micros = end.tv_usec - start.tv_usec;
	cout << "PerMessage Total Time: " << micros << " microseconds" << endl;
}

void* 
perMessageFTP(void* arg)
{
	PerMessage* pm = (PerMessage*) arg;
	
	for (int i = 0; i < SIZE; ++i)
	{
		Message* mesg = new Message(text, SIZE);
		pm->ftpSend(FTP_ID, mesg);
		usleep(SLEEP);
	}	
}

void* 
perMessageTelnet(void* arg)
{
	PerMessage* pm = (PerMessage*) arg;
	for (int i = 0; i < SIZE; ++i)
	{
		Message* mesg = new Message(text, SIZE);
		pm->telnetSend(TELNET_ID, mesg);
		usleep(SLEEP);
	}
}

void* 
perMessageDNS(void* arg)
{
	PerMessage* pm = (PerMessage*) arg;
	for (int i = 0; i < SIZE; ++i)
	{
		Message* mesg = new Message(text, SIZE);
		pm->dnsSend(DNS_ID, mesg);
		usleep(SLEEP);
	}
}

void* 
perMessageRDP(void* arg)
{
	PerMessage* pm = (PerMessage*) arg;
	for (int i = 0; i < SIZE; ++i)
	{
		Message* mesg = new Message(text, SIZE);
		pm->rdpSend(RDP_ID, mesg);
		usleep(SLEEP);
	}
}


// PerProtocol Apps -----------------------------------------------------------

void* 
perProtocolFTP(void* arg)
{
    PerProtocol* perProto = (PerProtocol*) arg;
    for (int i = 0; i < SIZE; i++)
    {
        Message* msg = new Message(text, SIZE);
        PerProtocol::pipe_unit ftp;
        ftp.protocol_id = 5;
        ftp.msg = msg;

        pthread_mutex_lock(perProto->ftp_send_pipe.pipe_mutex);

	        write(perProto->ftp_send_pipe.pipe_d[1], (char*) &ftp, sizeof(PerProtocol::pipe_unit));

        pthread_mutex_unlock(perProto->ftp_send_pipe.pipe_mutex);

        usleep(SLEEEP);
    }
}

void* 
perProtocolTelnet(void* arg)
{
    PerProtocol* perProto = (PerProtocol*) arg;
    for (int i = 0; i < SIZE; i++)
    {
        Message* msg = new Message(text, SIZE);
        PerProtocol::pipe_unit tel;
        tel.protocol_id = 6;
        tel.msg = msg;

        pthread_mutex_lock(perProto->tel_send_pipe.pipe_mutex);

	        write(perProto->tel_send_pipe.pipe_d[1], (char*) &tel, sizeof(PerProtocol::pipe_unit));

        pthread_mutex_unlock(perProto->tel_send_pipe.pipe_mutex);
    
        usleep(SLEEEP);
    }
}

void* 
perProtocolDNS(void* arg)
{
    PerProtocol* perProto = (PerProtocol*) arg;
    for (int i = 0; i < SIZE; i++)
    {
        Message* msg = new Message(text, SIZE);
        PerProtocol::pipe_unit dns;
        dns.protocol_id = 5;
        dns.msg = msg;

        pthread_mutex_lock(perProto->dns_send_pipe.pipe_mutex);

	        write(perProto->dns_send_pipe.pipe_d[1], (char*) &dns, sizeof(PerProtocol::pipe_unit));

        pthread_mutex_unlock(perProto->dns_send_pipe.pipe_mutex);

        usleep(SLEEEP);
    }
}

void* 
perProtocolRDP(void* arg)
{
    PerProtocol* perProto = (PerProtocol*) arg;
    for (int i = 0; i < SIZE; i++)
    {
        Message* msg = new Message(text, SIZE);
        PerProtocol::pipe_unit rdp;
        rdp.protocol_id = 5;
        rdp.msg = msg;

        pthread_mutex_lock(perProto->rdp_send_pipe.pipe_mutex);

	        write(perProto->rdp_send_pipe.pipe_d[1], (char*) &rdp, sizeof(PerProtocol::pipe_unit));

        pthread_mutex_unlock(perProto->rdp_send_pipe.pipe_mutex);

        usleep(SLEEEP);
    }
}
