#ifndef PROTOCOL_HEADERS
#define PROTOCOL_HEADERS

#define ETHERNET_ID 1
#define IP_ID       2
#define TCP_ID      3
#define UDP_ID      4
#define FTP_ID      5
#define TELNET_ID   6
#define RDP_ID      7
#define DNS_ID      8

typedef struct {
    int hlp;
    char otherInfo[12];
    int length;
} IPHeader;

typedef struct {
    int hlp;
    char otherInfo[8];
    int length;
} ethHeader;

typedef struct {
    int hlp;
    char otherInfo[4];
    int length;
} TCPHeader;

typedef struct {
    int hlp;
    char otherInfo[4];
    int length;
} UDPHeader;

typedef struct {
    int hlp;
    char otherInfo[12];
    int length;
} RDPHeader;

typedef struct {
    int hlp;
    char otherInfo[8];
    int length;
} DNSHeader;

typedef struct {
    int hlp;
    char otherInfo[8];
    int length;
} FTPHeader;

typedef struct {
    int hlp;
    char otherInfo[8];
    int length;
} telnetHeader;

#endif
