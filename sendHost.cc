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
#define	LINELEN		128
#define USAGE	"Usage: ./sendHost <host> <port>\n"

int errexit(const char *format, ...);
int udpSocket();
void sendUdp(int sock, char* message, const char* host, const char* port);
void getMesg(int sock);


int main(int argc, char const *argv[]) {
	if (argc < 3)
	{
		fprintf(stderr, USAGE);
		exit(1);
	}
	const char *host = argv[1];
	const char *port = argv[2];
	
	int sock = udpSocket();
	char message[LINELEN];
	while(fgets(message, sizeof(message), stdin)) {
		sendUdp(sock, message, host, port);
		memset(message, 0, sizeof(message));	
	}

	close(sock);
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
	myAddr.sin_port = htons(0);

	if(bind(sock, (struct sockaddr *)&myAddr, sizeof(myAddr)) < 0) {
		errexit("Could not bind: %s", strerror(errno));
	}
	return sock;
 }

void sendUdp(int sock, char* message, const char* host, const char* port) {

	struct sockaddr_in destAddr;
	memset((char *)&destAddr, 0, sizeof(destAddr));
 	destAddr.sin_family = AF_INET;
 	destAddr.sin_port = htons((unsigned short)atoi(port));
 	destAddr.sin_addr.s_addr = inet_addr(host);

	if(sendto(sock, message, strlen(message), 0, (struct sockaddr *)&destAddr, sizeof(destAddr)) < 0) {
		fprintf(stderr, "Failed to send UDP message: %s\n", strerror(errno));
	}
}

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








