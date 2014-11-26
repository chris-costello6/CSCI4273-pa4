#include "ppp.h"
#include "updsocket.cpp"

#include <unistd.h>
#include <string.h>
#include <iostream>

using namespace std;

PPP::PPP(char in[], int out){
    // Start communications on sockets first
    start_com(in, out);

    // For testing
    cout << "Building pipes" << endl;

    // Create pipes for threads
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

    // For testing
    cout << "Initializing mutex locks" << endl;

    // Initialize mutex locks on pipes
    pthread_mutex_t ftp_send_mut = PTHREAD_MUTEX_INITIALIZER;
    ftp_send_pipe.pipe_mutex = &ftp_send_mut;
    pthread_mutex_t ftp_recv_mut = PTHREAD_MUTEX_INITIALIZER;
    ftp_recv_pipe.pipe_mutex = &ftp_recv_mut;

    pthread_mutex_t tel_send_mut = PTHREAD_MUTEX_INITIALIZER;
    tel_send_pipe.pipe_mutex = &tel_send_mut;
    pthread_mutex_t tel_recv_mut = PTHREAD_MUTEX_INITIALIZER;
    tel_recv_pipe.pipe_mutex = &tel_recv_mut;

    pthread_mutex_t rdp_send_mut = PTHREAD_MUTEX_INITIALIZER;
    rdp_send_pipe.pipe_mutex = &rdp_send_mut;
    pthread_mutex_t rdp_recv_mut = PTHREAD_MUTEX_INITIALIZER;
    rdp_recv_pipe.pipe_mutex = &rdp_recv_mut;

    pthread_mutex_t dns_send_mut = PTHREAD_MUTEX_INITIALIZER;
    dns_send_pipe.pipe_mutex = &dns_send_mut;
    pthread_mutex_t dns_recv_mut = PTHREAD_MUTEX_INITIALIZER;
    dns_recv_pipe.pipe_mutex = &dns_recv_mut;

    pthread_mutex_t tcp_send_mut = PTHREAD_MUTEX_INITIALIZER;
    tcp_send_pipe.pipe_mutex = &tcp_send_mut;
    pthread_mutex_t tcp_recv_mut = PTHREAD_MUTEX_INITIALIZER;
    tcp_recv_pipe.pipe_mutex = &tcp_recv_mut;

    pthread_mutex_t udp_send_mut = PTHREAD_MUTEX_INITIALIZER;
    udp_send_pipe.pipe_mutex = &udp_send_mut;
    pthread_mutex_t udp_recv_mut = PTHREAD_MUTEX_INITIALIZER;
    udp_recv_pipe.pipe_mutex = &udp_recv_mut;

    pthread_mutex_t ip_send_mut = PTHREAD_MUTEX_INITIALIZER;
    ip_send_pipe.pipe_mutex = &ip_send_mut;
    pthread_mutex_t ip_recv_mut = PTHREAD_MUTEX_INITIALIZER;
    ip_recv_pipe.pipe_mutex = &ip_recv_mut;

    pthread_mutex_t eth_send_mut = PTHREAD_MUTEX_INITIALIZER;
    eth_send_pipe.pipe_mutex = &eth_send_mut;
    pthread_mutex_t eth_recv_mut = PTHREAD_MUTEX_INITIALIZER;
    eth_recv_pipe.pipe_mutex = &eth_recv_mut;

    // For testing
    cout << "Creating thread pool" << endl;

    // Create thread pool
    m_thread_pool = new ThreadPool(16);

    // Create thread ids
    pthread_t tid_1, tid_2, tid_3, tid_4, tid_5, tid_6, tid_7, tid_8, tid_9;
    pthread_t tid_10, tid_11, tid_12, tid_13, tid_14, tid_15, tid_16, tid_17;

    // Create vars for threads
    int err;

    // For testing
    cout << "Spooling up threads" << endl;

    // start a thread for ethernet up
    err = pthread_create(&tid_1, NULL, PPP::eth_recv, (void*) this);
    if (err != 0) {
        cout << "eth_recv() failed: " << err << endl;
        exit(1);
    }

    // start a thread for ethernet down
    err = pthread_create(&tid_2, NULL, PPP::eth_send, (void*) this);
    if (err != 0) {
        cout << "eth_send() failed: " << err << endl;
        exit(1);
    }

    // start a thread for ip up
    err = pthread_create(&tid_3, NULL, PPP::IP_recv, (void*) this);
    if (err != 0) {
        cout << "IP_recv() failed: " << err << endl;
        exit(1);
    }

    // start a thread for ip down
    err = pthread_create(&tid_4, NULL, PPP::IP_send, (void*) this);
    if (err != 0) {
        cout << "IP_send() failed: " << err << endl;
        exit(1);
    }

    // start a thread for udp up
    err = pthread_create(&tid_5, NULL, PPP::UDP_recv, (void*) this);
    if (err != 0) {
        cout << "UDP_recv() failed: " << err << endl;
        exit(1);
    }

    // start a thread for udp down
    err = pthread_create(&tid_6, NULL, PPP::UDP_send, (void*) this);
    if (err != 0) {
        cout << "UDP_recv() failed: " << err << endl;
        exit(1);
    }

    // start a thread for tcp up
    err = pthread_create(&tid_7, NULL, PPP::TCP_recv, (void*) this);
    if (err != 0) {
        cout << "TCP_recv() failed: " << err << endl;
        exit(1);
    }

    // start a thread for tcp down
    err = pthread_create(&tid_8, NULL, PPP::TCP_send, (void*) this);
    if (err != 0) {
        cout << "TCP_send() failed: " << err << endl;
        exit(1);
    }

    // start a thread for ftp up
    err = pthread_create(&tid_9, NULL, PPP::FTP_recv, (void*) this);
    if (err != 0) {
        cout << "FTP_recv() failed: " << err << endl;
        exit(1);
    }

    // start a thread for ftp down
    err = pthread_create(&tid_10, NULL, PPP::FTP_send, (void*) this);
    if (err != 0) {
        cout << "FTP_send() failed: " << err << endl;
        exit(1);
    }

    // start a thread for tel up
    err = pthread_create(&tid_11, NULL, PPP::tel_recv, (void*) this);
    if (err != 0) {
        cout << "tel_recv() failed: " << err << endl;
        exit(1);
    }

    // start a thread for tel down
    err = pthread_create(&tid_12, NULL, PPP::tel_send, (void*) this);
    if (err != 0) {
        cout << "tel_send() failed: " << err << endl;
        exit(1);
    }

    // start a thread for rdp up
    err = pthread_create(&tid_13, NULL, PPP::RDP_recv, (void*) this);
    if (err != 0) {
        cout << "RDP_recv() failed: " << err << endl;
        exit(1);
    }

    // start a thread for rdp down
    err = pthread_create(&tid_14, NULL, PPP::RDP_send, (void*) this);
    if (err != 0) {
        cout << "RDP_send() failed: " << err << endl;
        exit(1);
    }

    // start a thread for dns up
    err = pthread_create(&tid_15, NULL, PPP::DNS_recv, (void*) this);
    if (err != 0) {
        cout << "dns_recv() failed: " << err << endl;
        exit(1);
    }

    // start a thread for dns down
    err = pthread_create(&tid_16, NULL, PPP::DNS_send, (void*) this);
    if (err != 0) {
        cout << "dns_send() failed: " << err << endl;
        exit(1);
    }

    // start a thread for receiving messages
    err = pthread_create(&tid_17, NULL, PPP::msg_recv, (void*) this);
    if (err != 0) {
        cout << "msg_recv() failed: " << err << endl;
        exit(1);
    }

    // For testing
    cout << "Threads all spooled up" << endl;
}

PPP::~PPP()
{
    delete m_thread_pool;
}

/*
 * Function to start UDP connections
 *
 * Char in[] takes char array of desired port for inbound communications
 *
 * Int out takes an integer respresentation of the socket on the other client
 */
void PPP::start_com(char in[], int out){
    recv_sock = updSocket(in);
    send_sock = out;

    // For testing
    cout << "Port Number Recevied: " << in << endl;
}

/*
 * Function to put in receive client mode
 * 
 * Parameter is socket number of the server
 */
void* PPP::msg_recv(void* arg)
{
    PPP* ppp = (PPP*) arg;
    char msg_buf[1024];
    struct sockaddr_in cliaddr;
    socklen_t len;
    int n;

    // For testing
    cout << "Message receive thread running" << endl;

    while (1) {
        memset(&msg_buf, 0, sizeof(msg_buf));
        len = sizeof(cliaddr);

        // For testing
        cout << "Waiting for message" << endl;

        // Read message from socket
        n = recvfrom(ppp->recv_sock, msg_buf, 1024, 0, (struct sockaddr *)&cliaddr, &len);
        Message* msg = new Message(msg_buf, n);
        
        // Acquire mutex lock on pipe
        pthread_mutex_lock(ppp->eth_recv_pipe.pipe_mutex);

        // Write to pipe
        write(ppp->eth_recv_pipe.pipe_d[1], msg, sizeof(msg));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(ppp->eth_recv_pipe.pipe_mutex);
    }
}

void PPP::msg_send(Message* msg, int protocol_id){
    pipe_unit send_pipe;
    send_pipe.protocol_id = protocol_id;
    send_pipe.msg = msg;

    // For testing
    cout << "Testing send_pipe: " << send_pipe.protocol_id << endl;

    if (protocol_id == 5) {
        // Acquire mutex lock on pipe
        pthread_mutex_lock(ftp_send_pipe.pipe_mutex);

        // Write to ftp send pipe
        write(ftp_send_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(ftp_send_pipe.pipe_mutex);
    }

    else if (protocol_id == 6){
        // Acquire mutex lock on pipe
        pthread_mutex_lock(tel_send_pipe.pipe_mutex);

        // Write to ftp send pipe
        write(tel_send_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(tel_send_pipe.pipe_mutex);
    }

    else if (protocol_id == 7){
        // Acquire mutex lock on pipe
        pthread_mutex_lock(rdp_send_pipe.pipe_mutex);

        // Write to ftp send pipe
        write(rdp_send_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(rdp_send_pipe.pipe_mutex);
    }

    else if (protocol_id == 8){
        // For testing
        cout << "In DNS catch of msg_send" << endl;

        // Acquire mutex lock on pipe
        pthread_mutex_lock(dns_send_pipe.pipe_mutex);

        // For testing
        cout << "DNS mutex aquired" << endl;

        // Write to ftp send pipe
        write(dns_send_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

        // For testing
        cout << "DNS message written to pipe" << endl;

        // Remove mutex lock on pipe
        pthread_mutex_unlock(dns_send_pipe.pipe_mutex);
    }

    else{
        cout << "Invalid protocol_id" << endl;
    }
}

void* PPP::eth_send(void* arg){
    PPP* ppp = (PPP*) arg;
    char msg_buf[1024];
    struct sockaddr_in serv_sin;
    socklen_t len;
    
    while(1){
        Message* msg;
        pipe_unit* read_pipe;
        memset(&msg_buf, 0, sizeof(msg_buf));
        len = sizeof(serv_sin);

        // Wait until read succeeds
        read(ppp->eth_send_pipe.pipe_d[0], (pipe_unit*) read_pipe, sizeof(pipe_unit));

        // Store message in variable
        msg = read_pipe->msg;

        // Create new header
        eth_header* h = new eth_header;
        h->hlp = read_pipe->protocol_id;
        h->m_size = msg->msgLen();
        
        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(eth_header));

        // Flaten message to buffer
        msg->msgFlat(msg_buf);

        // Send out over network
        sendto(ppp->send_sock, msg_buf, msg->msgLen(), 0, (struct sockaddr *)&serv_sin, len);
    }


}

void* PPP::eth_recv(void* arg){
    PPP* ppp = (PPP*) arg;
    
    while(1){
        Message* msg;
        pipe_unit* read_pipe;

        read(ppp->eth_recv_pipe.pipe_d[0], (pipe_unit*) read_pipe, sizeof(pipe_unit));

        // Strip headers
        msg = read_pipe->msg;
        int protocol_id = atoi(msg->msgStripHdr(2));
        msg->msgStripHdr(10);

        // Create new pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 1;
        send_pipe.msg = msg;

        if (protocol_id == 2){
            // Lock mutex
            pthread_mutex_lock(ppp->ip_recv_pipe.pipe_mutex);

            // Write to pipe
            write(ppp->ip_recv_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

            // Unlock mutex
            pthread_mutex_unlock(ppp->ip_recv_pipe.pipe_mutex);
        }
        else{
            cout << "Error invalid protocol going up from eth" << endl;
        }
    }
}

void* PPP::IP_send(void* arg){
    PPP* ppp = (PPP*) arg;

    while(1){
        Message* msg;
        pipe_unit* read_pipe;

        // Wait for message to send
        read(ppp->ip_send_pipe.pipe_d[0], (pipe_unit*) read_pipe, sizeof(pipe_unit));

        // Store message in variable
        msg = read_pipe->msg;

        // Create new header
        IP_header* h = new IP_header;
        h->hlp = read_pipe->protocol_id;
        h->m_size = msg->msgLen();
        
        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(IP_header));

        // Build new pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 2;
        send_pipe.msg = msg;

        // Acquire mutex lock on pipe
        pthread_mutex_lock(ppp->eth_send_pipe.pipe_mutex);

        // Write to eth send pipe
        write(ppp->eth_send_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(ppp->eth_send_pipe.pipe_mutex);
    }
}

void* PPP::IP_recv(void* arg){
    PPP* ppp = (PPP*) arg;
    
    while(1){
        Message* msg;
        pipe_unit* read_pipe;

        read(ppp->ip_recv_pipe.pipe_d[0], (pipe_unit*) read_pipe, sizeof(pipe_unit));

        // Strip headers
        msg = read_pipe->msg;
        int protocol_id = atoi(msg->msgStripHdr(2));
        msg->msgStripHdr(14);

        // Create new pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 2;
        send_pipe.msg = msg;

        if (protocol_id == 3){
            // Lock mutex
            pthread_mutex_lock(ppp->tcp_recv_pipe.pipe_mutex);

            // Write to pipe
            write(ppp->tcp_recv_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

            // Unlock mutex
            pthread_mutex_unlock(ppp->tcp_recv_pipe.pipe_mutex);
        }
        else if (protocol_id == 4){
            // Lock mutex
            pthread_mutex_lock(ppp->udp_recv_pipe.pipe_mutex);

            // Write to pipe
            write(ppp->udp_recv_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

            // Unlock mutex
            pthread_mutex_unlock(ppp->udp_recv_pipe.pipe_mutex);
        }
        else{
            cout << "Error invalid protocol going up from ip" << endl;
        }
    }
}

void* PPP::TCP_send(void* arg){
    PPP* ppp = (PPP*) arg;

    while(1){
        Message* msg;
        pipe_unit* read_pipe;

        // Wait for message to send
        read(ppp->tcp_send_pipe.pipe_d[0], (pipe_unit*) read_pipe, sizeof(pipe_unit));

        // Store message in variable
        msg = read_pipe->msg;

        // Create new header
        TCP_header* h = new TCP_header;
        h->hlp = read_pipe->protocol_id;
        h->m_size = msg->msgLen();
        
        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(TCP_header));

        // Build new pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 3;
        send_pipe.msg = msg;

        // Acquire mutex lock on pipe
        pthread_mutex_lock(ppp->ip_send_pipe.pipe_mutex);

        // Write to eth send pipe
        write(ppp->ip_send_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(ppp->ip_send_pipe.pipe_mutex);
    }
}

void* PPP::TCP_recv(void* arg){
    PPP* ppp = (PPP*) arg;
    
    while(1){
        Message* msg;
        pipe_unit* read_pipe;

        read(ppp->tcp_recv_pipe.pipe_d[0], (pipe_unit*) read_pipe, sizeof(pipe_unit));

        // Strip headers
        msg = read_pipe->msg;
        int protocol_id = atoi(msg->msgStripHdr(2));
        msg->msgStripHdr(6);

        // Create new pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 3;
        send_pipe.msg = msg;

        if (protocol_id == 5){
            // Lock mutex
            pthread_mutex_lock(ppp->ftp_recv_pipe.pipe_mutex);

            // Write to pipe
            write(ppp->ftp_recv_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

            // Unlock mutex
            pthread_mutex_unlock(ppp->ftp_recv_pipe.pipe_mutex);
        }
        else if (protocol_id == 6){
            // Lock mutex
            pthread_mutex_lock(ppp->tel_recv_pipe.pipe_mutex);

            // Write to pipe
            write(ppp->tel_recv_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

            // Unlock mutex
            pthread_mutex_unlock(ppp->tel_recv_pipe.pipe_mutex);
        }
        else{
            cout << "Error invalid protocol going up from tcp" << endl;
        }
    }
}

void* PPP::UDP_send(void* arg){
    PPP* ppp = (PPP*) arg;

    while(1){
        Message* msg;
        pipe_unit* read_pipe;

        // Wait for message to send
        read(ppp->udp_send_pipe.pipe_d[0], (pipe_unit*) read_pipe, sizeof(pipe_unit));

        // Store message in variable
        msg = read_pipe->msg;

        // Create new header
        UDP_header* h = new UDP_header;
        h->hlp = read_pipe->protocol_id;
        h->m_size = msg->msgLen();
        
        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(UDP_header));

        // Build new pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 4;
        send_pipe.msg = msg;

        // Acquire mutex lock on pipe
        pthread_mutex_lock(ppp->ip_send_pipe.pipe_mutex);

        // Write to eth send pipe
        write(ppp->ip_send_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(ppp->ip_send_pipe.pipe_mutex);
    }
}

void* PPP::UDP_recv(void* arg){
    PPP* ppp = (PPP*) arg;
    
    while(1){
        Message* msg;
        pipe_unit* read_pipe;

        read(ppp->udp_recv_pipe.pipe_d[0], (pipe_unit*) read_pipe, sizeof(pipe_unit));

        // Strip headers
        msg = read_pipe->msg;
        int protocol_id = atoi(msg->msgStripHdr(2));
        msg->msgStripHdr(6);

        // Create new pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 4;
        send_pipe.msg = msg;

        if (protocol_id == 7){
            // Lock mutex
            pthread_mutex_lock(ppp->rdp_recv_pipe.pipe_mutex);

            // Write to pipe
            write(ppp->rdp_recv_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

            // Unlock mutex
            pthread_mutex_unlock(ppp->rdp_recv_pipe.pipe_mutex);
        }
        else if (protocol_id == 8){
            // Lock mutex
            pthread_mutex_lock(ppp->dns_recv_pipe.pipe_mutex);

            // Write to pipe
            write(ppp->dns_recv_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

            // Unlock mutex
            pthread_mutex_unlock(ppp->dns_recv_pipe.pipe_mutex);
        }
        else{
            cout << "Error invalid protocol going up from udp" << endl;
        }
    }
}

void* PPP::FTP_send(void* arg){
    PPP* ppp = (PPP*) arg;

    while(1){
        Message* msg;
        pipe_unit* read_pipe;

        // Wait for message to send
        read(ppp->ftp_send_pipe.pipe_d[0], (pipe_unit*) read_pipe, sizeof(pipe_unit));

        // Store message in variable
        msg = read_pipe->msg;

        // Create new header
        ftp_header* h = new ftp_header;
        h->m_size = msg->msgLen();
        
        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(ftp_header));

        // Build new pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 5;
        send_pipe.msg = msg;

        // Acquire mutex lock on pipe
        pthread_mutex_lock(ppp->tcp_send_pipe.pipe_mutex);

        // Write to eth send pipe
        write(ppp->tcp_send_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(ppp->tcp_send_pipe.pipe_mutex);
    }
}

void* PPP::FTP_recv(void* arg){
    PPP* ppp = (PPP*) arg;
    
    while(1){
        Message* msg;
        pipe_unit* read_pipe;

        read(ppp->ftp_recv_pipe.pipe_d[0], (pipe_unit*) read_pipe, sizeof(pipe_unit));

        // Strip headers
        msg = read_pipe->msg;
        msg->msgStripHdr(12);

        // Copy message to buffer and terminate line
        char* msg_buf = new char[1024];
        msg->msgFlat(msg_buf);
        msg_buf[msg->msgLen()] = '\n';

        // Print to user
        cout << "FTP Message Recevied: " << msg_buf;

        // Clean up
        delete msg_buf;
    }
}

void* PPP::tel_send(void* arg){
        PPP* ppp = (PPP*) arg;

    while(1){
        Message* msg;
        pipe_unit* read_pipe;

        // Wait for message to send
        read(ppp->tel_send_pipe.pipe_d[0], (pipe_unit*) read_pipe, sizeof(pipe_unit));

        // Store message in variable
        msg = read_pipe->msg;

        // Create new header
        tel_header* h = new tel_header;
        h->m_size = msg->msgLen();
        
        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(tel_header));

        // Build new pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 6;
        send_pipe.msg = msg;

        // Acquire mutex lock on pipe
        pthread_mutex_lock(ppp->tcp_send_pipe.pipe_mutex);

        // Write to eth send pipe
        write(ppp->tcp_send_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(ppp->tcp_send_pipe.pipe_mutex);
    }
}

void* PPP::tel_recv(void* arg){
    PPP* ppp = (PPP*) arg;
    
    while(1){
        Message* msg;
        pipe_unit* read_pipe;

        read(ppp->tel_recv_pipe.pipe_d[0], (pipe_unit*) read_pipe, sizeof(pipe_unit));

        // Strip headers
        msg = read_pipe->msg;
        msg->msgStripHdr(12);

        // Copy message to buffer and terminate line
        char* msg_buf = new char[1024];
        msg->msgFlat(msg_buf);
        msg_buf[msg->msgLen()] = '\n';

        // Print to user
        cout << "Telnet Message Recevied: " << msg_buf;

        // Clean up
        delete msg_buf;
    }
}

void* PPP::RDP_send(void* arg){
        PPP* ppp = (PPP*) arg;

    while(1){
        Message* msg;
        pipe_unit* read_pipe;

        // Wait for message to send
        read(ppp->rdp_send_pipe.pipe_d[0], (pipe_unit*) read_pipe, sizeof(pipe_unit));

        // Store message in variable
        msg = read_pipe->msg;

        // Create new header
        RDP_header* h = new RDP_header;
        h->m_size = msg->msgLen();
        
        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(RDP_header));

        // Build new pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 7;
        send_pipe.msg = msg;

        // Acquire mutex lock on pipe
        pthread_mutex_lock(ppp->udp_send_pipe.pipe_mutex);

        // Write to eth send pipe
        write(ppp->udp_send_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

        // Remove mutex lock on pipe
        pthread_mutex_unlock(ppp->udp_send_pipe.pipe_mutex);
    }
}

void* PPP::RDP_recv(void* arg){
    PPP* ppp = (PPP*) arg;
    
    while(1){
        Message* msg;
        pipe_unit* read_pipe;

        read(ppp->rdp_recv_pipe.pipe_d[0], (pipe_unit*) read_pipe, sizeof(pipe_unit));

        // Strip headers
        msg = read_pipe->msg;
        msg->msgStripHdr(16);

        // Copy message to buffer and terminate line
        char* msg_buf = new char[1024];
        msg->msgFlat(msg_buf);
        msg_buf[msg->msgLen()] = '\n';

        // Print to user
        cout << "RDP Message Recevied: " << msg_buf;

        // Clean up
        delete msg_buf;
    }
}

void* PPP::DNS_send(void* arg){
    PPP* ppp = (PPP*) arg;

    while(1){
        Message* msg;
        pipe_unit* read_pipe;

        // For testing
        cout << "Waiting for DNS message to send" << endl;

        // Wait for message to send
        read(ppp->dns_send_pipe.pipe_d[0], (pipe_unit*)read_pipe, sizeof(pipe_unit));

        // For testing
        cout << "DNS message read for sending" << endl;
        cout << "Protocol ID test: " << read_pipe->protocol_id << endl;

        // Store message in variable
        msg = read_pipe->msg;

        // For testing
        cout << "DNS message stored in local var" << endl;

        // Create new header
        DNS_header* h = new DNS_header;
        h->m_size = msg->msgLen();

        // For testing
        cout << "DNS header created" << endl;
        
        // Add header to message
        msg->msgAddHdr((char*) h, sizeof(DNS_header));

        // For testing
        cout << "DNS header added to message" << endl;

        // Build new pipe unit
        pipe_unit send_pipe;
        send_pipe.protocol_id = 8;
        send_pipe.msg = msg;

        // For testing
        cout << "DNS pipe unit built" << endl;

        // Acquire mutex lock on pipe
        pthread_mutex_lock(ppp->udp_send_pipe.pipe_mutex);

        // For testing
        cout << "UDP send mutex lock aquired" << endl;

        // Write to eth send pipe
        write(ppp->udp_send_pipe.pipe_d[1], (char*) &send_pipe, sizeof(pipe_unit));

        // For testing
        cout << "DNS message written to UDP" << endl;

        // Remove mutex lock on pipe
        pthread_mutex_unlock(ppp->udp_send_pipe.pipe_mutex);

        // For testing
        cout << "UDP send mutex lock released" << endl;
    }
}

void* PPP::DNS_recv(void* arg){
    PPP* ppp = (PPP*) arg;
    
    while(1){
        Message* msg;
        pipe_unit* read_pipe;

        read(ppp->dns_recv_pipe.pipe_d[0], (pipe_unit*) read_pipe, sizeof(pipe_unit));

        // Strip headers
        msg = read_pipe->msg;
        msg->msgStripHdr(12);

        // Copy message to buffer and terminate line
        char* msg_buf = new char[1024];
        msg->msgFlat(msg_buf);
        msg_buf[msg->msgLen()] = '\n';

        // Print to user
        cout << "DNS Message Recevied: " << msg_buf;

        // Clean up
        delete msg_buf;
    }
}
