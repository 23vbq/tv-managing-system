#include "clientsocket.h"

// Constructors

ClientSocket::ClientSocket(){
    m_client_fd = -2;
    m_address.sin_family = AF_INET;
    m_addrlen = sizeof(m_address);
    isConnected = false;
}

ClientSocket::~ClientSocket(){
    syslog(LOG_INFO, "Closing ClientSocket");
    if (m_client_fd > 0)
        Disconnect();
    close(m_client_fd);
}

// Private functions

void ClientSocket::CreateSocket(){
    // Create client fd
    if ((m_client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        throw "Creating client socket fd failed";
    }
    syslog(LOG_INFO, "Successfully created ClientSocket socket");
}
void ClientSocket::CloseSocket(){
    close(m_client_fd);
    m_client_fd = -2;
}

// Public functions

bool ClientSocket::Connect(const string &address, const uint16_t &port){
    // Create socket
    if (m_client_fd < 0){
        try {
            CreateSocket();
        } catch (const char* e){
            syslog(LOG_ERR, "[ClientSocket] %s", e);
            syslog(LOG_DEBUG, "Error while connecting to %s at %i", &address[0], port);
            return false;
        }
    }
    // Handle already connected
    if (isConnected){
        syslog(LOG_WARNING, "Trying to connect while connected [%s:%i] => [%s:%i]", inet_ntoa(m_address.sin_addr), ntohs(m_address.sin_port) &address[0], port);
        // FIXME validate connection with send and response
        return false;
    }
    // Create connection
    m_address.sin_port = htons(port);
    if (inet_pton(AF_INET, &address[0], &m_address.sin_addr) <= 0){
        syslog(LOG_ERR, "Invalid address %s with port %i", &address[0], port);
        CloseSocket();
        isConnected = false;
        return false;
    }
    if (connect(m_client_fd, (sockaddr*)&m_address, m_addrlen) < 0){
        syslog(LOG_NOTICE, "Connection failed to %s at port %i [Error: %i]", &address[0], port, errno);
        CloseSocket();
        isConnected = false;
        return false;
    }
    syslog(LOG_INFO, "Successfully connected to %s at port %i", &address[0], port);
    isConnected = true;
    return true;
}
bool ClientSocket::Send(const string& message){
    // Check if socket exists
    if (m_client_fd < 0){
        syslog(LOG_DEBUG, "Cannot send message becouse socket not exists [%s:%i]", inet_ntoa(m_address.sin_addr), ntohs(m_address.sin_port));
        return false;
    }
    // Handle not connected
    if (!isConnected){
        syslog(LOG_DEBUG, "Cannot send message becouse socket is not connected [%s:%i]", inet_ntoa(m_address.sin_addr), ntohs(m_address.sin_port));
        return false;
    }
    // Send message
    size_t message_len = message.length();
    if (send(m_client_fd, &message[0], message_len, MSG_NOSIGNAL) != message_len){
        CloseSocket();
        isConnected = false;
        syslog(LOG_ERR, "Error %i on sending message to [%s:%i]", errno, inet_ntoa(m_address.sin_addr), ntohs(m_address.sin_port));
        return false;
    }
    syslog(LOG_DEBUG, "Message sent to [%s:%i]", inet_ntoa(m_address.sin_addr), ntohs(m_address.sin_port));
    return true;
}
bool ClientSocket::Read(string &result){
    if ((m_readsize = read(m_client_fd, m_readbuff, _READBUFF_LEN -1)) < 1){
        result = "";
        isConnected = false;
        CloseSocket();
        syslog(LOG_ERR, "Error on reading from [%s:%i]", inet_ntoa(m_address.sin_addr), ntohs(m_address.sin_port));
        return false;
    }
    result = string(m_readbuff, m_readsize);
    syslog(LOG_DEBUG, "Successfully read from [%s:%i]", inet_ntoa(m_address.sin_addr), ntohs(m_address.sin_port));
    return true;
}
void ClientSocket::Disconnect(){
    Send("DISCON");
    CloseSocket();
    isConnected = false;
}
bool ClientSocket::IsConnected(){
    return isConnected;
}