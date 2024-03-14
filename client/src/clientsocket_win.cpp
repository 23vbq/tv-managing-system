#include "clientsocket_win.h"

extern Logger* m_log;

// Constructors

ClientSocket::ClientSocket(){
    int iResult;
    iResult = WSAStartup(MAKEWORD(2,2), &m_wsaData);
    if (iResult != 0){
        throw "WSAStartup failed: " + iResult;
    }
    m_conSock = INVALID_SOCKET;
    // FIXME test
    const char* testmsg = "TEST hello 2";
    addrinfo *result = NULL, *ptr = NULL, hints;
    // Initialize hints
    ZeroMemory( &hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    m_log->Log(Logger::LOG_INFO, "Successfully created client socket");
    // Resolve address
    iResult = getaddrinfo("192.168.121.132", "5555", &hints, &result);
    if (iResult != 0)
        throw "Getaddrinfo failed: " + iResult;
    // Connect to valid address // FIXME to rewrite to while (ptr && socketerror?)
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next){
        m_conSock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (m_conSock == INVALID_SOCKET){
            int err = WSAGetLastError();
            WSACleanup();
            throw "Socket failed: " + err;
        }
        iResult = connect(m_conSock, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR){
            closesocket(m_conSock);
            m_conSock = INVALID_SOCKET;
            continue;
        }
        break;
    }
    freeaddrinfo(result);
    // To do innej funkcji i bez throwa ale zwracanie wyniku or sth
    if (m_conSock == INVALID_SOCKET){
        WSACleanup();
        throw "Unable to connect to server";
    }
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