#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include <vector>
#include <syslog.h>

using namespace std;

class ServerSocket
{
private:
    int m_server_fd;
    sockaddr_in m_address;
    socklen_t m_addrlen;
    int m_opt = 1;

    vector<int> m_connections;
    void Disconnect(int);
public:
    ServerSocket(uint16_t);
    ~ServerSocket();

    void Handle();
};

#endif