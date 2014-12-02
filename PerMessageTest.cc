#include <stdio.h>
#include "PerMessage.h"
int main() {
	int port1 = 3001;
	int port2 = 3002;
	PerMessage *p1;
	p1 = new PerMessage(port1, port2);
}