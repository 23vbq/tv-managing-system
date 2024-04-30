#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#define _READBUFF_LEN 1024

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <syslog.h>

using namespace std;

class ClientSocket
{
private:
    int m_client_fd;
    sockaddr_in m_address;
    socklen_t m_addrlen;

    char m_readbuff[_READBUFF_LEN];

public:
    ClientSocket();
    ~ClientSocket();

    bool Connect(const string&, const uint16_t&);
    bool Send(const string&);
    bool Read(string&);
    void Disconnect();
    bool IsConnected();
};

#endif