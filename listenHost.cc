#include <sys/types.h>
#include <sys/socket.h>

#include <sys/errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define PORT 		8866
#define	BUFSIZE		4096

int errexit(const char *format, ...);
int udpSocket();
void getMesg(int sock);


int main() {	
	int sock = udpSocket();
	printf("listening on port %d\n", PORT);
	getMesg(sock);
}

/*
 * udpSocket - create and bind a udp socket
 */
int udpSocket() {
 	struct sockaddr_in myAddr;

	int sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock < 0) {
		errexit("Could not get socket: %s", strerror(errno));
	}

	memset((char *)&myAddr, 0, sizeof(myAddr));
	myAddr.sin_family = AF_INET;
	myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myAddr.sin_port = htons(PORT);

	if(bind(sock, (struct sockaddr *)&myAddr, sizeof(myAddr)) < 0) {
		errexit("Could not bind: %s", strerror(errno));
	}
	return sock;
 }

/*
	Receive incoming udp messages on specified socket.
*/
void getMesg(int sock) {
	char incomingMesg[BUFSIZE];
	struct sockaddr_in theirAddr;
	socklen_t len;
	recvfrom(sock , incomingMesg, BUFSIZE, 0, (struct sockaddr *)&theirAddr, &len);
	printf("%s\n", incomingMesg);
}


/*------------------------------------------------------------------------
 * errexit - print an error message and exit
 *------------------------------------------------------------------------
 */
int errexit(const char *format, ...) {
        va_list args;

        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
        exit(1);
}








