#include "clientsocket_win.h"

extern Logger* m_log;

// Constructors

ClientSocket::ClientSocket(){
    // Initialize variables
    m_conSock = INVALID_SOCKET;
    m_connected = false;
    // Initialize hints
    ZeroMemory( &m_addrConHints, sizeof(m_addrConHints));
    m_addrConHints.ai_family = AF_INET;
    m_addrConHints.ai_socktype = SOCK_STREAM;
    m_addrConHints.ai_protocol = IPPROTO_TCP;
    int iResult;
    iResult = WSAStartup(MAKEWORD(2,2), &m_wsaData);
    if (iResult != 0){
        throw "WSAStartup failed: " + iResult;
    }
    // FIXME test
    const char* testmsg = "TEST hello 2";
    m_log->Log(Logger::LOG_INFO, "Successfully created client socket");
    Connect();
    char read[1024] = {0};
    do { // To jest zjechane wtf kto tak robi, bedzie normalnie w funkcji read - potem handle
        iResult = recv(m_conSock, read, 1024, 0);
        if (iResult > 0)
            std::cout<<read<<"\n";
        else if (iResult < 0){
            throw "recv failed with error: " + WSAGetLastError();
        } else break;
        iResult = send(m_conSock, testmsg, (int)strlen(testmsg), 0);
        if (iResult == SOCKET_ERROR){
            m_log->Log(Logger::LOG_ERROR, "Send failed: " + WSAGetLastError());
            break;
        }
    } while (iResult > 0);
    closesocket(m_conSock);
    WSACleanup();
}

// Public functions

void ClientSocket::Connect(){ // Should i instead throwing log error and return some error code to user and then display msg based on code?
    int iResult;
    addrinfo *addrResult = NULL, *addrPtr = NULL;
    // Resolve address
    iResult = getaddrinfo("192.168.121.132", "5555", &m_addrConHints, &addrResult);
    if (iResult != 0)
        throw "Getaddrinfo failed: " + iResult;
    addrPtr = addrResult;
    while (addrPtr){
        m_conSock = socket(addrPtr->ai_family, addrPtr->ai_socktype, addrPtr->ai_protocol);
        if (m_conSock == INVALID_SOCKET){
            int err = WSAGetLastError();
            WSACleanup();
            throw "Socket failed: " + err;
        }
        iResult = connect(m_conSock, addrPtr->ai_addr, (int)addrPtr->ai_addrlen);
        // Break if connection is valid
        if (iResult == 0)
            break;
        closesocket(m_conSock);
        m_conSock = INVALID_SOCKET;
        addrPtr = addrPtr->ai_next;
    }
    freeaddrinfo(addrResult);
    if (m_conSock == INVALID_SOCKET){
        WSACleanup();
        m_connected = false;
        throw "Unable to connect to server";
    }
    m_connected = true;
}