#include "serversocket.h"

ServerSocket::ServerSocket(uint16_t port){
    // Initialize client sockets
    for (int i = 0; i < MAX_CLIENTS; i++){
        m_sockets[i] = 0;
    }
    // Create server socket fd
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
    // Listening
    if (listen(m_server_fd, 3) < 0){
        throw "listen";
    }
    // FIXME Tests
    // Accept
    
    // std::string hello = "Test";
    // send(new_sock, &hello[0], hello.length(), 0);
    // close(new_sock);
    // close(m_server_fd);
}
ServerSocket::~ServerSocket(){
    /*for (int conn : m_connections){
        syslog(LOG_INFO, "Closing connection: %i", conn);
        close(conn);
    }
    syslog(LOG_INFO, "Closing socket");*/
    for (int i = 0; i < MAX_CLIENTS; i++){
        close(m_sockets[i]);
    }
    close(m_server_fd);
}

void ServerSocket::Disconnect(int connection){
    using namespace std;
    close(connection);
    /*int n = m_connections.size();
    for (int i = 0; i < n; i++){
        if (m_connections.at(i) == connection){
            m_connections.erase(m_connections.begin() + i);
            syslog(LOG_INFO, "Successfully disconnected: %i", connection);
        }
    }*/
}

void ServerSocket::Handle(){
    using namespace std;
    // Prepare sockets fd list
    FD_ZERO(&m_readfds);
    FD_SET(m_server_fd, &m_readfds);
    int max_sd = m_server_fd;
    int sd, activity, new_socket;
    for (int i = 0; i < MAX_CLIENTS; i++){
        sd = m_sockets[i];
        if (sd > 0)
            FD_SET(sd, &m_readfds);
        if (sd > max_sd)
            max_sd = sd;
    }
    // Get active fd
    activity = select(max_sd + 1, &m_readfds, NULL, NULL, NULL);
    if ((activity < 0) && (errno != EINTR)){
        syslog(LOG_ERR, "FD select error!");
    }
    // Handle new connection
    if (FD_ISSET(m_server_fd, &m_readfds)){
        if ((new_socket = accept(m_server_fd, (sockaddr*)&m_address, &m_addrlen)) < 0){
            throw "Error on handling new connection";
        }
        syslog(LOG_INFO, "Accepted connection [fd: %i, ip: %s, port: %i]", new_socket, inet_ntoa(m_address.sin_addr), ntohs(m_address.sin_port));
        if (send(new_socket, &hello_msg[0], hello_msg.length(), 0) != hello_msg.length()){
            syslog(LOG_ERR, "Error on sending hello message!");
        }
        for (int i = 0; i < MAX_CLIENTS; i++){
            if (m_sockets[i] == 0){
                m_sockets[i] = new_socket;
                return;
            }
        }
    }
    // Handle existing connections
    char buffer[1024] = {0};
    size_t readlen;
    for (int i = 0; i < MAX_CLIENTS; i++){
        sd = m_sockets[i];
        if (FD_ISSET(sd, &m_readfds)){
            // Handle disconnection
            if ((readlen = read(sd, buffer, 1024 - 1)) == 0){
                close(sd);
            } else{
                char rev[1024] = {0};
                for (int i = 0; i < readlen; i++)
                    rev[i] = buffer[readlen - i - 1];
                send(sd, rev, readlen, 0);
            }
        }
    }
}

/*void ServerSocket::Handle(){
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
}*/