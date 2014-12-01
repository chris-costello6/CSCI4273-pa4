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
#include "Threadpool.h"
#include "Message.h"

#define	BUFSIZE		4096

int errexit(const char *format, ...);
int udpSocket(int port);
char* getMesg(int sock);
void sendUdp(int sock, char* message, const char* host, const char* port);

/*
	udpSocket - create and bind a udp socket
 */
int udpSocket(int port) {
 	struct sockaddr_in myAddr;

	int sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock < 0) {
		errexit("Could not get socket: %s", strerror(errno));
	}

	memset((char *)&myAddr, 0, sizeof(myAddr));
	myAddr.sin_family = AF_INET;
	myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myAddr.sin_port = htons(port);

	if(::bind(sock, (struct sockaddr *)&myAddr, sizeof(myAddr)) != 0 ) {
		errexit("Could not bind: %s", strerror(errno));
	}
	return sock;
 }

/*
	Receive incoming udp messages on specified socket.
*/
char* getMesg(int sock) {
	char incomingMesg[BUFSIZE];
	struct sockaddr_in theirAddr;
	socklen_t len;
	recvfrom(sock , incomingMesg, BUFSIZE, 0, (struct sockaddr *)&theirAddr, &len);
	return incomingMesg;
}
/*
	Send outgoing udp messages to specified host on specified socket and port.
*/
void sendUdp(int sock, char* message, const char* host, const char* port) {
	struct hostent *hp;
	hp = gethostbyname(host);
	if (!hp) { 
		fprintf(stderr, "could not obtain address of %s\n", host); 
		return; 
	}
	struct sockaddr_in destAddr;
	memset((char *)&destAddr, 0, sizeof(destAddr));
 	destAddr.sin_family = AF_INET;
 	destAddr.sin_port = htons((unsigned short)atoi(port));
 	memcpy((void *)&destAddr.sin_addr, hp->h_addr_list[0], hp->h_length);

	if(sendto(sock, message, strlen(message), 0, (struct sockaddr *)&destAddr, sizeof(destAddr)) < 0) {
		fprintf(stderr, "Failed to send UDP message: %s\n", strerror(errno));
	}
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








