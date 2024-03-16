#include "clientsocket_win.h"

extern Logger* m_log;

// Constructors

ClientSocket::ClientSocket(){
    // Initialize variables
    m_conSock = INVALID_SOCKET;
    m_connected = false;
    m_iResult = 0;
    // Initialize hints
    ZeroMemory( &m_addrConHints, sizeof(m_addrConHints));
    m_addrConHints.ai_family = AF_INET;
    m_addrConHints.ai_socktype = SOCK_STREAM;
    m_addrConHints.ai_protocol = IPPROTO_TCP;
    m_iResult = WSAStartup(MAKEWORD(2,2), &m_wsaData);
    if (m_iResult != 0){
        throw "WSAStartup failed: " + m_iResult;
    }
    m_log->Log(Logger::LOG_INFO, "Successfully created client socket");
    // FIXME test
    /*const char* testmsg = "TEST hello 2";
    Connect();
    char read[1024] = {0};
    do { // To jest zjechane wtf kto tak robi, bedzie normalnie w funkcji read - potem handle
        memset(read, 0, 1024);
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
    WSACleanup();*/
}
ClientSocket::~ClientSocket(){
    closesocket(m_conSock);
    WSACleanup();
    m_log->Log(Logger::LOG_INFO, "Destructed client socket");
}

// Public functions

void ClientSocket::Connect(){ // Should i instead throwing log error and return some error code to user and then display msg based on code?
    addrinfo *addrResult = NULL, *addrPtr = NULL;
    // Resolve address
    m_iResult = getaddrinfo("192.168.121.132", "5555", &m_addrConHints, &addrResult);
    if (m_iResult != 0)
        throw "Getaddrinfo failed: " + m_iResult;
    addrPtr = addrResult;
    while (addrPtr){
        m_conSock = socket(addrPtr->ai_family, addrPtr->ai_socktype, addrPtr->ai_protocol);
        if (m_conSock == INVALID_SOCKET){
            int err = WSAGetLastError();
            WSACleanup();
            throw "Socket failed: " + err;
        }
        m_iResult = connect(m_conSock, addrPtr->ai_addr, (int)addrPtr->ai_addrlen);
        // Break if connection is valid
        if (m_iResult == 0)
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
int ClientSocket::Send(std::string message){ // maybe return int error code too
    if (!isConnected())
        return CS_NOTCONN;
    m_iResult = send(m_conSock, &message[0], message.length(), 0);
    if (m_iResult == SOCKET_ERROR)
        return CS_SOCKERR; // should i throw? and close socket
    return CS_SUCCESS;
}
int ClientSocket::Receive(std::string& result){
    if (!isConnected())
        return CS_NOTCONN;
    memset(m_readbuff, 0, CS_READBUFF_LEN);
    m_iResult = recv(m_conSock, m_readbuff, CS_READBUFF_LEN - 1, 0); // -1 for last NULL byte
    if (m_iResult < 0)
        return CS_SOCKERR; // should i throw? and close socket
    result = std::string(m_readbuff);
    return CS_SUCCESS;
}
bool ClientSocket::isConnected(){
    return m_connected;
}