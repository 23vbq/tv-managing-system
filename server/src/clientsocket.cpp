#include "clientsocket.h"

// Constructors

ClientSocket::ClientSocket(){
    // Create client fd
    if ((m_client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        throw "Creating client socket fd failed";
    }
    m_address.sin_family = AF_INET;
    m_addrlen = sizeof(m_address);
    syslog(LOG_INFO, "Successfully created ClientSocket");
}

ClientSocket::~ClientSocket(){
    syslog(LOG_INFO, "Closing ClientSocket");
    if (IsConnected())
        Disconnect();
    close(m_client_fd);
}

// Public functions

bool ClientSocket::Connect(const string &address, const uint16_t &port){
    m_address.sin_port = htons(port);
    if (inet_pton(AF_INET, &address[0], &m_address.sin_addr) <= 0){
        syslog(LOG_ERR, "Invalid address %s with port %i", &address[0], port);
        return false;
    }
    if (connect(m_client_fd, (sockaddr*)&m_address, m_addrlen) < 0){
        syslog(LOG_NOTICE, "Connection failed to %s at port %i", &address[0], port);
        return false;
    }
    syslog(LOG_INFO, "Successfully connected to %s at port %i", &address[0], port);
    return true;
}