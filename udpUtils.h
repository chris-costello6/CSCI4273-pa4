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

#define	BUFSIZE		4096

int errexit(const char *format, ...);
int udpSocket(int port);
int getUdpMesg(int sock, char* buffer, int bufferlen);
void sendUdp(int sock, char* message, int mesgLen, const char* host, int port);

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
	Copies message into provided buffer and returns 
	the number of bytes read. Assumes caller zeroed out
	the buffer beforehand.
*/
int getUdpMesg(int sock, char* buffer, int bufferlen) {
	// char* incomingMesg = new char[BUFSIZE];
	struct sockaddr_in theirAddr;
	socklen_t len;
	int n = recvfrom(sock, buffer, bufferlen, 0, 
		    		(struct sockaddr *)&theirAddr, &len);
	// Trim to minimum size.
	// int i = 0;
	// char c;
	// do {
	// 	c = incomingMesg[i];
	// 	i++;
	// } while (c != '\0');

	// char* retVal = (char *) malloc(i * sizeof(char));
	// memcpy(retVal, incomingMesg, i);
	// return retVal;
	return n;
}
/*
	Send outgoing udp messages to specified host on specified socket and port.
*/
void sendUdp(int sock, char* message, int mesgLen, const char* host, int port) {
	struct hostent *hp;
	hp = gethostbyname(host);
	if (!hp) { 
		fprintf(stderr, "could not obtain address of %s\n", host); 
		return; 
	}
	struct sockaddr_in destAddr;
	memset((char *)&destAddr, 0, sizeof(destAddr));
 	destAddr.sin_family = AF_INET;
 	destAddr.sin_port = htons((unsigned short)port);
 	memcpy((void *)&destAddr.sin_addr, hp->h_addr_list[0], hp->h_length);

	if(sendto(sock, message, mesgLen, 0, (struct sockaddr *)&destAddr, sizeof(destAddr)) < 0) {
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








