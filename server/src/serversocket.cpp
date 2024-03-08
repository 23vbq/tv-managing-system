#include "serversocket.h"

// Static strings
const string ServerSocket::SMSG_HELLO = "Hello TMSD\r\n";
const string ServerSocket::SMSG_REFNRE = "Connection refused: ";

ServerSocket::ServerSocket(uint16_t port){
    // Initialize client sockets
    for (int i = 0; i < MAX_SOCKETS; i++){
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
    m_address.sin_addr.s_addr = INADDR_ANY; // TODO ip from config
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
}
ServerSocket::~ServerSocket(){
    /*for (int conn : m_connections){
        syslog(LOG_INFO, "Closing connection: %i", conn);
        close(conn);
    }
    syslog(LOG_INFO, "Closing socket");*/
    for (int i = 0; i < MAX_SOCKETS; i++){ // TODO check is valid to new connection handling
        close(m_sockets[i]);
    }
    close(m_server_fd);
}

bool ServerSocket::AcceptConnectionHandle(){
    int new_socket;
    if (FD_ISSET(m_server_fd, &m_readfds)){
        if ((new_socket = accept(m_server_fd, (sockaddr*)&m_address, &m_addrlen)) < 0){
            throw "Error on handling new connection";
        }
        syslog(LOG_INFO, "Accepted connection [fd: %i, ip: %s, port: %i]", new_socket, inet_ntoa(m_address.sin_addr), ntohs(m_address.sin_port));
        if (send(new_socket, &SMSG_HELLO[0], SMSG_HELLO.length(), 0) != SMSG_HELLO.length()){
            syslog(LOG_ERR, "Error on sending hello message!");
        }
        bool inArray = false;
        for (int i = 0; i < MAX_SOCKETS; i++){
            if (m_sockets[i] == 0){
                m_sockets[i] = new_socket;
                inArray = true;
                break; // Break wazny bo inaczej zapelniam caly array
            }
        }
        if (!inArray) {
            string smsg_ref = SMSG_REFNRE + "Too many clients connected.\r\n";
            syslog(LOG_WARNING, "%s", &smsg_ref[0]);
            send(new_socket, &smsg_ref[0], smsg_ref.length(), 0);
            close(new_socket);
        }
        return true;
    }
    return false;
}
void ServerSocket::LostConnectionHandle(int connection){
    using namespace std;
    getpeername(connection, (sockaddr*)&m_address, &m_addrlen); // FIXME when last connection wasn't localhost, and localhost disconnects it return last connection (in my example gateway)
    close(connection);
    for (int i = 0; i < MAX_SOCKETS; i++){
        if (m_sockets[i] == connection)
            m_sockets[i] = 0;
    }
    syslog(LOG_WARNING, "Lost connection [fd: %i, ip: %s, port: %i]", connection, inet_ntoa(m_address.sin_addr), ntohs(m_address.sin_port));
}

void ServerSocket::Handle(){
    using namespace std;
    // Prepare sockets fd list
    FD_ZERO(&m_readfds);
    FD_SET(m_server_fd, &m_readfds);
    int max_sd = m_server_fd;
    int sd, activity;
    for (int i = 0; i < MAX_SOCKETS; i++){
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
    if (AcceptConnectionHandle())
        return;
    // Handle existing connections
    char buffer[1024] = {0};
    size_t readlen;
    for (int i = 0; i < MAX_SOCKETS; i++){
        sd = m_sockets[i];
        if (FD_ISSET(sd, &m_readfds)){
            // Handle disconnection
            if ((readlen = read(sd, buffer, 1024 - 1)) == 0){
                LostConnectionHandle(sd);
            } else{
                char rev[1024] = {0};
                for (int i = 0; i < readlen; i++)
                    rev[i] = buffer[readlen - i - 1];
                send(sd, rev, readlen, 0);
            }
        }
    }
}