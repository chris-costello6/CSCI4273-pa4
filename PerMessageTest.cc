#include <stdio.h>
#include "PerMessage.h"
#define USAGE "./PerMessageTest <send|listen>"
#define SEND "send"
#define LISTEN "listen"
#define port1 44444
#define port2 45555

int main(int argc, char const *argv[])
{

		// PerMessage p2(port2, port1);
		// sleep (2);
		// char* mesg = "aaaaaaaaaaa";
		// Message m(mesg, strlen(mesg));
		// p2.ethernetSend(IP_ID, &m);

	if(argc < 2) {
		fprintf(stderr, "%s\n", USAGE);
		exit(1);
	}
	if (strcmp(argv[1], LISTEN) == 0) {
		PerMessage p1(port1, port2);
		pause();
	} else if (strcmp(argv[1], SEND) == 0) {
		PerMessage p2(port2, port1);
		sleep (2);
		char *h1 = new char[5];
    	for (int i = 0; i < 5; i++) h1[i] = 'h';
		Message* m = new Message (h1, 5);
		p2.ethernetSend(IP_ID, m);
	} else {
		fprintf(stderr, "%s\n", USAGE);
		exit(1);
	}
	return 0;
}