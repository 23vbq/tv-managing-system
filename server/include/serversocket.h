#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <syslog.h>

using namespace std;

class ServerSocket
{
public:
    static const int MAX_SOCKETS = 2;
    static const string SMSG_HELLO;
    static const string SMSG_REFNRE;

private:
    int m_server_fd;
    sockaddr_in m_address;
    socklen_t m_addrlen;
    int m_opt = 1;

    fd_set m_readfds;
    int m_sockets[MAX_SOCKETS];

    //vector<int> m_connections;
    bool AcceptConnectionHandle();
    void Disconnect(int);
public:
    ServerSocket(uint16_t);
    ~ServerSocket();

    void Handle();
};

#endif