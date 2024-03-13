#include "clientsocket_win.h"

// Constructors

ClientSocket::ClientSocket(){
    int iResult;
    iResult = WSAStartup(MAKEWORD(2,2), &m_wsaData);
    if (iResult != 0){
        throw "WSAStartup failed: " + iResult;
    }
}