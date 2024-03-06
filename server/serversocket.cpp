#include "serversocket.h"

ServerSocket::ServerSocket(uint16_t port){
    // Create socket fd
    if ((m_server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        throw "Creating socket_fd failed";
    }
    if (setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &m_opt, sizeof(m_opt)) < 0){
        throw "setsockopt";
    }
    // Setup address struct
    m_address.sin_family = AF_INET;
    m_address.sin_addr.s_addr = INADDR_ANY;
    m_address.sin_port = htons(port);
    m_addrlen = sizeof(m_address);
    // Bind
    if (bind(m_server_fd, (sockaddr*)&m_address, m_addrlen) < 0){
        throw "Binding socket failed";
    }
    // FIXME Tests
    // Listening
    if (listen(m_server_fd, 3) < 0){
        throw "listen";
    }
    // Accept
    int new_sock;
    if ((new_sock = accept(m_server_fd, (sockaddr*)&m_address, &m_addrlen)) < 0){
        throw "accept";
    }
    std::string hello = "Test";
    send(new_sock, &hello[0], hello.length(), 0);
    close(new_sock);
    close(m_server_fd);
}