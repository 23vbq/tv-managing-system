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
    
    // std::string hello = "Test";
    // send(new_sock, &hello[0], hello.length(), 0);
    // close(new_sock);
    // close(m_server_fd);
}
ServerSocket::~ServerSocket(){
    for (int conn : m_connections){
        syslog(LOG_INFO, "Closing connection: %i", conn);
        close(conn);
    }
    syslog(LOG_INFO, "Closing socket");
    close(m_server_fd);
}

void ServerSocket::Disconnect(int connection){
    using namespace std;
    close(connection);
    int n = m_connections.size();
    for (int i = 0; i < n; i++){
        if (m_connections.at(i) == connection){
            m_connections.erase(m_connections.begin() + i);
            syslog(LOG_INFO, "Successfully disconnected: %i", connection);
        }
    }
}

void ServerSocket::Handle(){
    using namespace std;
    if (m_connections.size() == 0){
        int new_sock;
        if ((new_sock = accept(m_server_fd, (sockaddr*)&m_address, &m_addrlen)) < 0){
            throw "accept";
        }
        m_connections.push_back(new_sock);
        syslog(LOG_INFO, "Successfully connected: %i", new_sock);
    }
    char buffer[1024] = { 0 };
    size_t readlen;
    for (int conn : m_connections){
        readlen = read(conn, buffer, 1024 - 1);
        if (readlen < 1){
            syslog(LOG_WARNING, "Connection lost with: %i", conn);
            Disconnect(conn);
            continue;
        }
        // FIXME testing
        char rev[1024] = {0};
        for (int i = 0; i < readlen; i++)
            rev[i] = buffer[readlen - i - 1];
        send(conn, rev, readlen, 0);
    }
}