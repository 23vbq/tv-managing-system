#ifndef CLIENT_SOCKET_WIN_H
#define CLIENT_SOCKET_WIN_H

#define CS_SUCCESS 0
#define CS_NOTCONN (-1)
#define CS_SOCKERR (-2)

#define CS_READBUFF_LEN 1024

#include "logger.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

class ClientSocket{
private:
    WSADATA m_wsaData;
    SOCKET m_conSock;

    bool m_connected;
    int m_iResult;
    addrinfo m_addrConHints;
    char m_readbuff[CS_READBUFF_LEN];
public:
    ClientSocket();
    ~ClientSocket();

    void Connect(std::string, std::string);
    void Disconnect();
    int Send(std::string);
    int Receive(std::string&);
    bool isConnected();
};

#endif