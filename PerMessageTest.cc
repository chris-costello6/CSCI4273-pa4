#include <stdio.h>
#include "PerMessage.h"
#define USAGE "./PerMessageTest <send|listen|ethTest>"
#define SEND "send"
#define LISTEN "listen"
#define ETHTEST "ethTest"
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
	} else if(strcmp(argv[1], ETHTEST) == 0) {
		char* t = new char[10];
		for (int i = 0; i < 10; i++) t[i] = 'v';
		
		Message* mm = new Message(t, 10);
		
		ethHeader* e = new ethHeader;
		e->hlp = 5;
		for (int i = 0; i < 8; ++i) e->otherInfo[i] = 'q';
		e->length = mm->msgLen();
		cout << "===BEFORE===" << endl;
		cout << "hlp=" << e->hlp << " otherInfo=" << e->otherInfo
		<< " length=" << e->length << endl;

		mm->msgAddHdr((char*) e, sizeof(ethHeader));
		
		// char content[30];
		// mm->msgFlat(content);
		// cout << "content is " << content << " before sending" << endl;

		// ethHeader* stripped = (ethHeader*) mm->msgStripHdr(sizeof(ethHeader));

		// bzero(content, 30);
		// mm->msgFlat(content);
		// cout << "After strip, Content =" << content << endl;
		
		// cout << "===AFTER===" << endl;
		// cout << "hlp=" << stripped->hlp << " otherInfo=" << stripped->otherInfo
		// << " length=" << stripped->length << endl;

		PerMessage* p3 = new PerMessage();
		p3->ethernetRecv((void*)mm);
		
	} else{
		fprintf(stderr, "%s\n", USAGE);
		exit(1);
	}
	return 0;
}