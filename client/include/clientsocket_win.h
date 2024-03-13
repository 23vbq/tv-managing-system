#ifndef CLIENT_SOCKET_WIN_H
#define CLIENT_SOCKET_WIN_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

class ClientSocket{
private:
    WSADATA m_wsaData;
public:
    ClientSocket();
};

#endif