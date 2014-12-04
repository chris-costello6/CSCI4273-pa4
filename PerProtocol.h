#include "./ourLibs/Message.h"
#include "./newLibs/cj/threadpool.h"
#include "Headers.h"
#include "udpsocket.cpp"

#include <mutex>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

using namespace std;

typedef void (*function_pointer)(void*);

class PerProtocol
{
public:
    // Struct for sending to pipes
    struct pipe_unit {
        int protocol_id;
        Message *msg;
    };

    //Struct to store arguements for the resquester threads
    typedef struct {
        // Array for pipe descriptor
        int pipe_d[2];
     
        //Pointer to mutex
        pthread_mutex_t* pipe_mutex;
    } pipes;

    PerProtocol(char in[], char out[]);
    ~PerProtocol();
    
    void msg_send(Message* msg, int protocol_id);
    void start_com(char in[], char[]);

    // Allocate structs for application level pipes
    pipes ftp_send_pipe, tel_send_pipe, rdp_send_pipe, dns_send_pipe;

    // Mutext locks for application level send pipes
    pthread_mutex_t ftp_send_mut;
    pthread_mutex_t tel_send_mut;
    pthread_mutex_t rdp_send_mut;
    pthread_mutex_t dns_send_mut;

private:
    ThreadPool* m_thread_pool;

    // Functions for protocols
    static void* eth_send(void* arg);
    static void* eth_recv(void* arg);
    static void* IP_send(void* arg);
    static void* IP_recv(void* arg);
    static void* TCP_send(void* arg);
    static void* TCP_recv(void* arg);
    static void* UDP_send(void* arg);
    static void* UDP_recv(void* arg);
    static void* FTP_send(void* arg);
    static void* FTP_recv(void* arg);
    static void* tel_send(void* arg);
    static void* tel_recv(void* arg);
    static void* RDP_send(void* arg);
    static void* RDP_recv(void* arg);
    static void* DNS_send(void* arg);
    static void* DNS_recv(void* arg);

    // Functions for receiving messages
    static void* msg_recv(void* arg);

    // Allocate structs for pipes
    pipes ftp_recv_pipe, tel_recv_pipe, rdp_recv_pipe, dns_recv_pipe;
    pipes tcp_send_pipe, tcp_recv_pipe, udp_send_pipe, udp_recv_pipe;
    pipes ip_send_pipe, ip_recv_pipe, eth_send_pipe, eth_recv_pipe;

    // Allocate mutex locks for pipes
    pthread_mutex_t ftp_recv_mut;
    pthread_mutex_t tel_recv_mut;
    pthread_mutex_t rdp_recv_mut;
    pthread_mutex_t dns_recv_mut;
    pthread_mutex_t tcp_send_mut;
    pthread_mutex_t tcp_recv_mut;
    pthread_mutex_t udp_send_mut;
    pthread_mutex_t udp_recv_mut;
    pthread_mutex_t ip_send_mut;
    pthread_mutex_t ip_recv_mut;
    pthread_mutex_t eth_send_mut;
    pthread_mutex_t eth_recv_mut;

    int recv_sock, send_sock;

    char* m_send_port;
    char* m_recv_port;
};

PerProtocol::PerProtocol(char in[], char out[])
{

    // Start communications on sockets first
    start_com(in, out);


    // Create pipes for protocols ---------------------------------------------
    pipe(ftp_send_pipe.pipe_d);
    pipe(ftp_recv_pipe.pipe_d);

    pipe(tel_send_pipe.pipe_d);
    pipe(tel_recv_pipe.pipe_d);

    pipe(tcp_send_pipe.pipe_d);
    pipe(tcp_recv_pipe.pipe_d);

    pipe(rdp_send_pipe.pipe_d);
    pipe(rdp_recv_pipe.pipe_d);

    pipe(udp_send_pipe.pipe_d);
    pipe(udp_recv_pipe.pipe_d);

    pipe(dns_send_pipe.pipe_d);
    pipe(dns_recv_pipe.pipe_d);

    pipe(eth_send_pipe.pipe_d);
    pipe(eth_recv_pipe.pipe_d);

    pipe(ip_send_pipe.pipe_d);
    pipe(ip_recv_pipe.pipe_d);


    // Initialize pipe mutexes ------------------------------------------------
    pthread_mutex_init(&ftp_send_mut, NULL);
    ftp_send_pipe.pipe_mutex = &ftp_send_mut;
    pthread_mutex_init(&ftp_recv_mut, NULL);
    ftp_recv_pipe.pipe_mutex = &ftp_recv_mut;

    pthread_mutex_init(&tel_send_mut, NULL);
    tel_send_pipe.pipe_mutex = &tel_send_mut;
    pthread_mutex_init(&tel_recv_mut, NULL);
    tel_recv_pipe.pipe_mutex = &tel_recv_mut;

    pthread_mutex_init(&rdp_send_mut, NULL);
    rdp_send_pipe.pipe_mutex = &rdp_send_mut;
    pthread_mutex_init(&rdp_recv_mut, NULL);
    rdp_recv_pipe.pipe_mutex = &rdp_recv_mut;

    pthread_mutex_init(&dns_send_mut, NULL);
    dns_send_pipe.pipe_mutex = &dns_send_mut;
    pthread_mutex_init(&dns_recv_mut, NULL);
    dns_recv_pipe.pipe_mutex = &dns_recv_mut;

    pthread_mutex_init(&tcp_send_mut, NULL);
    tcp_send_pipe.pipe_mutex = &tcp_send_mut;
    pthread_mutex_init(&tcp_recv_mut, NULL);
    tcp_recv_pipe.pipe_mutex = &tcp_recv_mut;

    pthread_mutex_init(&udp_send_mut, NULL);
    udp_send_pipe.pipe_mutex = &udp_send_mut;
    pthread_mutex_init(&udp_recv_mut, NULL);
    udp_recv_pipe.pipe_mutex = &udp_recv_mut;

    pthread_mutex_init(&ip_send_mut, NULL);
    ip_send_pipe.pipe_mutex = &ip_send_mut;
    pthread_mutex_init(&ip_recv_mut, NULL);
    ip_recv_pipe.pipe_mutex = &ip_recv_mut;

    pthread_mutex_init(&eth_send_mut, NULL);
    eth_send_pipe.pipe_mutex = &eth_send_mut;
    pthread_mutex_init(&eth_recv_mut, NULL);
    eth_recv_pipe.pipe_mutex = &eth_recv_mut;


    // Declare threads --------------------------------------------------------
    pthread_t tid_1, tid_2, tid_3, tid_4, tid_5, tid_6, tid_7, tid_8, tid_9;
    pthread_t tid_10, tid_11, tid_12, tid_13, tid_14, tid_15, tid_16, tid_17;


    // Send threads to protocol functions -------------------------------------
    if (pthread_create(&tid_17, NULL, PerProtocol::msg_recv, (void*) this))
    {
        cout << "msg_recv() failed! " << endl;
        exit(1);
    }

    if (pthread_create(&tid_1, NULL, PerProtocol::eth_recv, (void*) this))
    {
        cout << "eth_recv() failed! " << endl;
        exit(1);
    }

    if (pthread_create(&tid_2, NULL, PerProtocol::eth_send, (void*) this))
    {
        cout << "eth_send() failed! " << endl;
        exit(1);
    }

    if (pthread_create(&tid_3, NULL, PerProtocol::IP_recv, (void*) this))
    {
        cout << "IP_recv() failed! " << endl;
        exit(1);
    }

    if (pthread_create(&tid_4, NULL, PerProtocol::IP_send, (void*) this))
    {
        cout << "IP_send() failed! " << endl;
        exit(1);
    }

    if (pthread_create(&tid_5, NULL, PerProtocol::UDP_recv, (void*) this))
    {
        cout << "UDP_recv() failed! " << endl;
        exit(1);
    }

    if (pthread_create(&tid_6, NULL, PerProtocol::UDP_send, (void*) this))
    {
        cout << "UDP_recv() failed! " << endl;
        exit(1);
    }

    if (pthread_create(&tid_7, NULL, PerProtocol::TCP_recv, (void*) this))
    {
        cout << "TCP_recv() failed! " << endl;
        exit(1);
    }

    if (pthread_create(&tid_8, NULL, PerProtocol::TCP_send, (void*) this))
    {
        cout << "TCP_send() failed! " << endl;
        exit(1);
    }

    if (pthread_create(&tid_9, NULL, PerProtocol::FTP_recv, (void*) this))
    {
        cout << "FTP_recv() failed! " << endl;
        exit(1);
    }

    if (pthread_create(&tid_10, NULL, PerProtocol::FTP_send, (void*) this))
    {
        cout << "FTP_send() failed! " << endl;
        exit(1);
    }

    if (pthread_create(&tid_11, NULL, PerProtocol::tel_recv, (void*) this))
    {
        cout << "tel_recv() failed! " << endl;
        exit(1);
    }

    if (pthread_create(&tid_12, NULL, PerProtocol::tel_send, (void*) this))
    {
        cout << "tel_send() failed! " << endl;
        exit(1);
    }

    if (pthread_create(&tid_13, NULL, PerProtocol::RDP_recv, (void*) this))
    {
        cout << "RDP_recv() failed! " << endl;
        exit(1);
    }

    if (pthread_create(&tid_14, NULL, PerProtocol::RDP_send, (void*) this))
    {
        cout << "RDP_send() failed! " << endl;
        exit(1);
    }

    if (pthread_create(&tid_15, NULL, PerProtocol::DNS_recv, (void*) this))
    {
        cout << "dns_recv() failed! " << endl;
        exit(1);
    }

    if (pthread_create(&tid_16, NULL, PerProtocol::DNS_send, (void*) this))
    {
        cout << "dns_send() failed! " << endl;
        exit(1);
    }
}


PerProtocol::~PerProtocol()
{

}

/*
 * Function to start UDP connections
 *
 * Char in[] takes char array of desired port for inbound communications
 *
 * Int out takes an integer respresentation of the socket on the other client
 */
void 
PerProtocol::start_com(char in[], char out[])
{
    recv_sock = updSocket(in);
    m_send_port = out;
}

/*
 * Function to put in receive client mode
 * 
 * Parameter is socket number of the server
 */
void* 
PerProtocol::msg_recv(void* arg)
{
    PerProtocol* ppp = (PerProtocol*) arg;
    struct sockaddr_in cliaddr;
    socklen_t len;
    int n;

    while (1) 
    {
        char* msg_buf = new char[1024];
        memset(msg_buf, 0, 1024);
        len = sizeof(cliaddr);

        // Read message from socket
        n = recvfrom(ppp->recv_sock, msg_buf, 1024, 0, (struct sockaddr *)&cliaddr, &len);
        Message* msg = new Message(msg_buf, n);

        // Build pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 0;
        send_pipe.msg = msg;
        
        pthread_mutex_lock(ppp->eth_recv_pipe.pipe_mutex);

            write(ppp->eth_recv_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

        pthread_mutex_unlock(ppp->eth_recv_pipe.pipe_mutex);
    }
}

void 
PerProtocol::msg_send(Message* msg, int protocol_id)
{
    if (protocol_id == 5) 
    {
        pipe_unit ftp;
        ftp.protocol_id = 5;
        ftp.msg = msg;

        pthread_mutex_lock(ftp_send_pipe.pipe_mutex);

            write(ftp_send_pipe.pipe_d[1], (char*) &ftp, sizeof(pipe_unit));

        pthread_mutex_unlock(ftp_send_pipe.pipe_mutex);
    }

    else if (protocol_id == 6)
    {
        pipe_unit tel;
        tel.protocol_id = 6;
        tel.msg = msg;
        
        pthread_mutex_lock(tel_send_pipe.pipe_mutex);

            write(tel_send_pipe.pipe_d[1], (char*) &tel, sizeof(pipe_unit));

        pthread_mutex_unlock(tel_send_pipe.pipe_mutex);
    }

    else if (protocol_id == 7)
    {
        pipe_unit rdp;
        rdp.protocol_id = 7;
        rdp.msg = msg;
        
        pthread_mutex_lock(rdp_send_pipe.pipe_mutex);

            write(rdp_send_pipe.pipe_d[1], (char*) &rdp, sizeof(pipe_unit));

        pthread_mutex_unlock(rdp_send_pipe.pipe_mutex);
    }

    else if (protocol_id == 8)
    {
        pipe_unit dns;
        dns.protocol_id = 8;
        dns.msg = msg;

        pthread_mutex_lock(dns_send_pipe.pipe_mutex);

            write(dns_send_pipe.pipe_d[1], (char*) &dns, sizeof(pipe_unit));

        pthread_mutex_unlock(dns_send_pipe.pipe_mutex);
    }

    else
        cout << "Invalid protocol_id" << endl;
}

void* 
PerProtocol::eth_send(void* arg)
{
    PerProtocol* ppp = (PerProtocol*) arg;
    struct sockaddr_in servaddr;
    socklen_t len;
    struct hostent *phe;    // pointer to host information entry

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;

    char* host = "localhost";

    // Map port number (char string) to port number (int)
    if ((servaddr.sin_port = htons((unsigned short)atoi(ppp->m_send_port))) == 0)
        errexit("can't get \"%s\" port number\n", ppp->m_send_port);

    // Map host name to IP address, allowing for dotted decimal
    if ((phe = gethostbyname(host)))
        memcpy(&servaddr.sin_addr, phe->h_addr, phe->h_length);
    else if ((servaddr.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE)
        errexit("can't get \"%s\" host entry\n", host);

    // Allocate a socket
    int upd_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (upd_sock < 0)
        errexit("can't create socket: %s\n", strerror(errno));
    
    while(1)
    {
        Message* msg;
        pipe_unit* read_pipe = new pipe_unit;

        pthread_mutex_unlock(ppp->eth_send_pipe.pipe_mutex);

            read(ppp->eth_send_pipe.pipe_d[0], (char*) read_pipe, sizeof(pipe_unit));

        pthread_mutex_lock(ppp->eth_send_pipe.pipe_mutex);

        
        msg = read_pipe->msg;

        // Create new header
        ethHeader* h = (ethHeader *) malloc( sizeof(ethHeader));
        h->hlp = read_pipe->protocol_id;
        h->length = msg->msgLen();

        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(ethHeader));

        // Flaten message to buffer
        char msg_buf[1024];
        memset(&msg_buf, 0, sizeof(msg_buf));
        msg->msgFlat(msg_buf);

        // Sent message to network
        if (sendto(upd_sock, msg_buf, msg->msgLen(), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
            printf("Error with sendto %s\n", strerror(errno));
    }
}

void* 
PerProtocol::eth_recv(void* arg)
{
    PerProtocol* ppp = (PerProtocol*) arg;
    
    while(1)
    {
        Message* msg;
        pipe_unit* read_pipe = new pipe_unit;

        pthread_mutex_unlock(ppp->eth_recv_pipe.pipe_mutex);

            read(ppp->eth_recv_pipe.pipe_d[0], (char*) read_pipe, sizeof(pipe_unit));

        pthread_mutex_lock(ppp->eth_recv_pipe.pipe_mutex);

        // Strip headers
        msg = read_pipe->msg;
        ethHeader* stripped = (ethHeader*)msg->msgStripHdr(sizeof(ethHeader));
        int protocol_id = stripped->hlp;

        // Create new pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 1;
        send_pipe.msg = msg;

        if (protocol_id == 2)
        {
            pthread_mutex_lock(ppp->ip_recv_pipe.pipe_mutex);

                write(ppp->ip_recv_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

            pthread_mutex_unlock(ppp->ip_recv_pipe.pipe_mutex);
        }
        else
            cout << "Error invalid protocol going up from eth" << endl;
    }
}

void* 
PerProtocol::IP_send(void* arg)
{
    PerProtocol* ppp = (PerProtocol*) arg;

    while(1)
    {
        Message* msg;
        pipe_unit* read_pipe = new pipe_unit;

        pthread_mutex_unlock(ppp->ip_send_pipe.pipe_mutex);

            read(ppp->ip_send_pipe.pipe_d[0], (char*) read_pipe, sizeof(pipe_unit));

        pthread_mutex_lock(ppp->ip_send_pipe.pipe_mutex);

        msg = read_pipe->msg;

        // Create new header
        IPHeader* h = (IPHeader *) malloc( sizeof(IPHeader));
        h->hlp = read_pipe->protocol_id;
        h->length = msg->msgLen();
        
        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(IPHeader));

        // Build new pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 2;
        send_pipe.msg = msg;

        pthread_mutex_lock(ppp->eth_send_pipe.pipe_mutex);

            write(ppp->eth_send_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

        pthread_mutex_unlock(ppp->eth_send_pipe.pipe_mutex);
    }
}

void* 
PerProtocol::IP_recv(void* arg)
{
    PerProtocol* ppp = (PerProtocol*) arg;
    
    while(1)
    {
        Message* msg;
        pipe_unit* read_pipe = new pipe_unit;

        pthread_mutex_unlock(ppp->ip_recv_pipe.pipe_mutex);

            read(ppp->ip_recv_pipe.pipe_d[0], (char*) read_pipe, sizeof(pipe_unit));

        pthread_mutex_lock(ppp->ip_recv_pipe.pipe_mutex);

        // Add header to message
        msg = read_pipe->msg;
        IPHeader* stripped = (IPHeader*)msg->msgStripHdr(sizeof(IPHeader));
        int protocol_id = stripped->hlp;

        // Create new pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 2;
        send_pipe.msg = msg;

        if (protocol_id == 3)
        {
            pthread_mutex_lock(ppp->tcp_recv_pipe.pipe_mutex);

                write(ppp->tcp_recv_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

            pthread_mutex_unlock(ppp->tcp_recv_pipe.pipe_mutex);
        }
        else if (protocol_id == 4)
        {
            pthread_mutex_lock(ppp->udp_recv_pipe.pipe_mutex);

                write(ppp->udp_recv_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

            pthread_mutex_unlock(ppp->udp_recv_pipe.pipe_mutex);
        }
        else
            cout << "Error invalid protocol going up from ip" << endl;
    }
}

void* 
PerProtocol::TCP_send(void* arg)
{
    PerProtocol* ppp = (PerProtocol*) arg;

    while(1)
    {
        Message* msg;
        pipe_unit* read_pipe = new pipe_unit;

        pthread_mutex_unlock(ppp->tcp_send_pipe.pipe_mutex);

            read(ppp->tcp_send_pipe.pipe_d[0], (char*) read_pipe, sizeof(pipe_unit));

        pthread_mutex_lock(ppp->tcp_send_pipe.pipe_mutex);

        
        msg = read_pipe->msg;

        // Create new header
        TCPHeader* h = (TCPHeader *) malloc( sizeof(TCPHeader));
        h->hlp = read_pipe->protocol_id;
        h->length = msg->msgLen();
        
        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(TCPHeader));

        // Build new pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 3;
        send_pipe.msg = msg;

        pthread_mutex_lock(ppp->ip_send_pipe.pipe_mutex);

            write(ppp->ip_send_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

        pthread_mutex_unlock(ppp->ip_send_pipe.pipe_mutex);
    }
}

void* 
PerProtocol::TCP_recv(void* arg)
{
    PerProtocol* ppp = (PerProtocol*) arg;
    
    while(1)
    {
        Message* msg;
        pipe_unit* read_pipe = new pipe_unit;

        pthread_mutex_unlock(ppp->tcp_recv_pipe.pipe_mutex);

            read(ppp->tcp_recv_pipe.pipe_d[0], (char*) read_pipe, sizeof(pipe_unit));

        pthread_mutex_lock(ppp->tcp_recv_pipe.pipe_mutex);

        // Strip headers
        msg = read_pipe->msg;
        TCPHeader* stripped = (TCPHeader*)msg->msgStripHdr(sizeof(TCPHeader));
        int protocol_id = stripped->hlp;

        // Create new pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 3;
        send_pipe.msg = msg;

        if (protocol_id == 5)
        {
            pthread_mutex_lock(ppp->ftp_recv_pipe.pipe_mutex);

                write(ppp->ftp_recv_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

            pthread_mutex_unlock(ppp->ftp_recv_pipe.pipe_mutex);
        }
        else if (protocol_id == 6)
        {
            pthread_mutex_lock(ppp->tel_recv_pipe.pipe_mutex);

                write(ppp->tel_recv_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

            pthread_mutex_unlock(ppp->tel_recv_pipe.pipe_mutex);
        }
        else
            cout << "Error invalid protocol going up from tcp" << endl;
    }
}

void* 
PerProtocol::UDP_send(void* arg)
{
    PerProtocol* ppp = (PerProtocol*) arg;
    
    while(1)
    {
        Message* msg;
        pipe_unit* read_pipe = new pipe_unit;

        pthread_mutex_unlock(ppp->udp_send_pipe.pipe_mutex);

            read(ppp->udp_send_pipe.pipe_d[0], (char*) read_pipe, sizeof(pipe_unit));

        pthread_mutex_lock(ppp->udp_send_pipe.pipe_mutex);

        
        msg = read_pipe->msg;

        // Create new header
        UDPHeader* h = (UDPHeader *) malloc( sizeof(UDPHeader));
        h->hlp = read_pipe->protocol_id;
        h->length = msg->msgLen();

        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(UDPHeader));

        // Build new pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 4;
        send_pipe.msg = msg;

        pthread_mutex_lock(ppp->ip_send_pipe.pipe_mutex);

            write(ppp->ip_send_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

        pthread_mutex_unlock(ppp->ip_send_pipe.pipe_mutex);
    }
}

void* 
PerProtocol::UDP_recv(void* arg)
{
    PerProtocol* ppp = (PerProtocol*) arg;
    
    while(1)
    {
        Message* msg;
        pipe_unit* read_pipe = new pipe_unit;

        pthread_mutex_unlock(ppp->udp_recv_pipe.pipe_mutex);

            read(ppp->udp_recv_pipe.pipe_d[0], (char*) read_pipe, sizeof(pipe_unit));

        pthread_mutex_lock(ppp->udp_recv_pipe.pipe_mutex);

        // Strip headers
        msg = read_pipe->msg;
        UDPHeader* stripped = (UDPHeader*)msg->msgStripHdr(sizeof(UDPHeader));
        int protocol_id = stripped->hlp;

        // Create new pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 4;
        send_pipe.msg = msg;

        if (protocol_id == 7)
        {
            pthread_mutex_lock(ppp->rdp_recv_pipe.pipe_mutex);

                write(ppp->rdp_recv_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

            pthread_mutex_unlock(ppp->rdp_recv_pipe.pipe_mutex);
        }
        else if (protocol_id == 8)
        {
            pthread_mutex_lock(ppp->dns_recv_pipe.pipe_mutex);

                write(ppp->dns_recv_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

            pthread_mutex_unlock(ppp->dns_recv_pipe.pipe_mutex);
        }
        else
            cout << "Error invalid protocol going up from udp" << endl;
    }
}

void* 
PerProtocol::FTP_send(void* arg)
{
    PerProtocol* ppp = (PerProtocol*) arg;

    while(1)
    {
        Message* msg;
        pipe_unit* read_pipe = new pipe_unit;

        pthread_mutex_unlock(ppp->ftp_send_pipe.pipe_mutex);

            read(ppp->ftp_send_pipe.pipe_d[0], (char*) read_pipe, sizeof(pipe_unit));

        pthread_mutex_lock(ppp->ftp_send_pipe.pipe_mutex);

        
        msg = read_pipe->msg;

        // Create new header
        FTPHeader* h = (FTPHeader *) malloc( sizeof(FTPHeader));
        h->length = msg->msgLen();
        
        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(FTPHeader));

        // Build new pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 5;
        send_pipe.msg = msg;

        pthread_mutex_lock(ppp->tcp_send_pipe.pipe_mutex);

            write(ppp->tcp_send_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

        pthread_mutex_unlock(ppp->tcp_send_pipe.pipe_mutex);
    }
}

void* 
PerProtocol::FTP_recv(void* arg)
{
    PerProtocol* ppp = (PerProtocol*) arg;
    
    while(1)
    {
        Message* msg;
        pipe_unit* read_pipe = new pipe_unit;

        pthread_mutex_unlock(ppp->ftp_recv_pipe.pipe_mutex);

            read(ppp->ftp_recv_pipe.pipe_d[0], (char*) read_pipe, sizeof(pipe_unit));

        pthread_mutex_lock(ppp->ftp_recv_pipe.pipe_mutex);

        // Strip headers
        msg = read_pipe->msg;
        msg->msgStripHdr(sizeof(FTPHeader));

        // Copy message to buffer and terminate line
        char* msg_buf = new char[1024];
        msg->msgFlat(msg_buf);
        msg_buf[msg->msgLen()] = '\n';

        cout << "FTP Message Recevied: " << msg_buf;

        delete msg_buf;
    }
}

void* 
PerProtocol::tel_send(void* arg)
{
        PerProtocol* ppp = (PerProtocol*) arg;

    while(1)
    {
        Message* msg;
        pipe_unit* read_pipe = new pipe_unit;

        pthread_mutex_unlock(ppp->tel_send_pipe.pipe_mutex);

            read(ppp->tel_send_pipe.pipe_d[0], (char*) read_pipe, sizeof(pipe_unit));

        pthread_mutex_lock(ppp->tel_send_pipe.pipe_mutex);

        
        msg = read_pipe->msg;

        // Create new header
        telnetHeader* h = (telnetHeader*) malloc( sizeof(telnetHeader));
        h->length = msg->msgLen();
        
        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(telnetHeader));

        // Build new pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 6;
        send_pipe.msg = msg;

        pthread_mutex_lock(ppp->tcp_send_pipe.pipe_mutex);

            write(ppp->tcp_send_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

        pthread_mutex_unlock(ppp->tcp_send_pipe.pipe_mutex);
    }
}

void* 
PerProtocol::tel_recv(void* arg)
{
    PerProtocol* ppp = (PerProtocol*) arg;
    
    while(1)
    {
        Message* msg;
        pipe_unit* read_pipe = new pipe_unit;

        pthread_mutex_unlock(ppp->tel_recv_pipe.pipe_mutex);

            read(ppp->tel_recv_pipe.pipe_d[0], (char*) read_pipe, sizeof(pipe_unit));

        pthread_mutex_lock(ppp->tel_recv_pipe.pipe_mutex);

        // Strip headers
        msg = read_pipe->msg;
        msg->msgStripHdr(sizeof(telnetHeader));

        // Copy message to buffer and terminate line
        char* msg_buf = new char[1024];
        msg->msgFlat(msg_buf);
        msg_buf[msg->msgLen()] = '\n';

        cout << "Telnet Message Recevied: " << msg_buf;

        delete msg_buf;
    }
}

void* 
PerProtocol::RDP_send(void* arg)
{
    PerProtocol* ppp = (PerProtocol*) arg;

    while(1)
    {
        Message* msg;
        pipe_unit* read_pipe = new pipe_unit;

        pthread_mutex_unlock(ppp->rdp_send_pipe.pipe_mutex);

            read(ppp->rdp_send_pipe.pipe_d[0], (char*) read_pipe, sizeof(pipe_unit));

        pthread_mutex_lock(ppp->rdp_send_pipe.pipe_mutex);

        
        msg = read_pipe->msg;

        // Create new header
        RDPHeader* h = (RDPHeader *) malloc( sizeof(RDPHeader));
        h->length = msg->msgLen();
        
        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(RDPHeader));

        // Build new pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 7;
        send_pipe.msg = msg;

        pthread_mutex_lock(ppp->udp_send_pipe.pipe_mutex);

            write(ppp->udp_send_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

        pthread_mutex_unlock(ppp->udp_send_pipe.pipe_mutex);
    }
}

void* 
PerProtocol::RDP_recv(void* arg)
{
    PerProtocol* ppp = (PerProtocol*) arg;
    
    while(1)
    {
        Message* msg;
        pipe_unit* read_pipe = new pipe_unit;

        pthread_mutex_unlock(ppp->rdp_recv_pipe.pipe_mutex);

            read(ppp->rdp_recv_pipe.pipe_d[0], (char*) read_pipe, sizeof(pipe_unit));

        pthread_mutex_lock(ppp->rdp_recv_pipe.pipe_mutex);

        // Strip headers
        msg = read_pipe->msg;
        msg->msgStripHdr(sizeof(RDPHeader));

        // Copy message to buffer and terminate line
        char* msg_buf = new char[1024];
        msg->msgFlat(msg_buf);
        msg_buf[msg->msgLen()] = '\n';

        cout << "RDP Message Recevied: " << msg_buf;

        delete msg_buf;
    }
}

void* 
PerProtocol::DNS_send(void* arg)
{
    PerProtocol* ppp = (PerProtocol*) arg;

    while(1)
    {
        Message* msg;
        pipe_unit* read_pipe = new pipe_unit;

        pthread_mutex_unlock(ppp->dns_send_pipe.pipe_mutex);

            read(ppp->dns_send_pipe.pipe_d[0], (char*)read_pipe, sizeof(pipe_unit));

        pthread_mutex_lock(ppp->dns_send_pipe.pipe_mutex);

        
        msg = read_pipe->msg;

        // Create new header
        DNSHeader* h = (DNSHeader *) malloc( sizeof(DNSHeader));
        h->length = msg->msgLen();

        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(DNSHeader));

        // Build new pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 8;
        send_pipe.msg = msg;

        pthread_mutex_lock(ppp->udp_send_pipe.pipe_mutex);

            write(ppp->udp_send_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

        pthread_mutex_unlock(ppp->udp_send_pipe.pipe_mutex);
    }
}

void* 
PerProtocol::DNS_recv(void* arg)
{
    PerProtocol* ppp = (PerProtocol*) arg;
    
    while(1)
    {
        Message* msg;
        pipe_unit* read_pipe = new pipe_unit;

        pthread_mutex_unlock(ppp->dns_recv_pipe.pipe_mutex);

            read(ppp->dns_recv_pipe.pipe_d[0], (char*) read_pipe, sizeof(pipe_unit));

        pthread_mutex_lock(ppp->dns_recv_pipe.pipe_mutex);

        // Strip headers
        msg = read_pipe->msg;
        msg->msgStripHdr(sizeof(DNSHeader));

        // Copy message to buffer and terminate line
        char* msg_buf = new char[1024];
        msg->msgFlat(msg_buf);
        msg_buf[msg->msgLen()] = '\n';

        cout << "DNS Message Recevied: " << msg_buf;

        delete msg_buf;
    }
}
