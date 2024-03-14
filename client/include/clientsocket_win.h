#ifndef CLIENT_SOCKET_WIN_H
#define CLIENT_SOCKET_WIN_H

#include "logger.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
// FIXME test
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

class ClientSocket{
private:
    WSADATA m_wsaData;
    SOCKET m_conSock;
public:
    ClientSocket();
};

#endif