#include "serversocket.h"

// Static variables

const string ServerSocket::SMSG_HELLO = "Hello TMS\r\n";
const string ServerSocket::SMSG_REFNRE = "Connection refused: ";
const string ServerSocket::SMSG_CLSD = "Connection closed by server\r\n";
const string ServerSocket::SMSG_AUTH_REQ = "AUTH_REQ";
const string ServerSocket::SOCKET_LOOP_IGNORE_SIG = "SOCKLOOPIGN";

// Constructors

ServerSocket::ServerSocket(bool* termination, uint16_t port, int maxConnections){
    s_termination = termination;
    MAX_SOCKETS = maxConnections;
    m_sockets = new int[MAX_SOCKETS];
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
    syslog(LOG_INFO, "Successfully created ServerSocket");
}
ServerSocket::~ServerSocket(){
    using namespace std;
    syslog(LOG_INFO, "Closing ServerSocket");
    for (int i = 0; i < MAX_SOCKETS; i++){ // TODO check is valid to new connection handling
        Disconnect(m_sockets[i], SMSG_CLSD);
    }
    delete [] m_sockets;
    close(m_server_fd);
    syslog(LOG_INFO, "Closed ServerSocket successfully");
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
    // FIXME when last connection wasn't localhost, and localhost disconnects it return last connection (in my example gateway)
    string sock_info = GetSocketInfo(connection);
    close(connection);
    for (int i = 0; i < MAX_SOCKETS; i++){
        if (m_sockets[i] == connection)
            m_sockets[i] = 0;
    }
    syslog(LOG_WARNING, "Lost connection %s", &sock_info[0]);
}

void ServerSocket::Handle(string (*responseCall)(char[], int, int)){
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
    try{
        activity = select(max_sd + 1, &m_readfds, NULL, NULL, NULL); // FIXME throws exception here but cannot catch it
    } catch (...){
        syslog(LOG_ERR, "activity");
        return;
    }
    if ((activity < 0) && (errno != EINTR)){
        syslog(LOG_ERR, "FD select error!");
    }
    // Handle new connection
    if (*s_termination || AcceptConnectionHandle())
        return;
    // Handle existing connections
    size_t readlen, datalen;
    string data;
    for (int i = 0; i < MAX_SOCKETS; i++){
        // FIXME should i clean buffer? length is obtained so maybe not
        sd = m_sockets[i];
        if (FD_ISSET(sd, &m_readfds)){
            // Handle disconnection
            if ((readlen = read(sd, m_readbuff, _READBUFF_LEN - 1)) == 0){
                LostConnectionHandle(sd);
            } else{
                data = responseCall(m_readbuff, readlen, sd);
                if (data.find(SOCKET_LOOP_IGNORE_SIG) != string::npos)
                    continue;
                datalen = data.length();
                if (send(sd, &data[0], datalen, 0) != datalen){
                    syslog(LOG_ERR, "Error on sending response message %s", &GetSocketInfo(sd)[0]);
                    LostConnectionHandle(sd); // Connection closed: Error on sending response message. // Result cannot be sent here
                } else{
                    // getpeername(sd, (sockaddr*)&m_address, &m_addrlen);
                    syslog(LOG_INFO, "Response sent %s", &GetSocketInfo(sd)[0]);
                }
            }
        }
    }
}
void ServerSocket::Disconnect(int connection, string reason){
    if (connection == 0)
        return;
    using namespace std;
    //getpeername(connection, (sockaddr*)&m_address, &m_addrlen);
    if (reason != "")
        send(connection, &reason[0], reason.length(), 0);
    close(connection);
    for (int i = 0; i < MAX_SOCKETS; i++){
        if (m_sockets[i] == connection)
            m_sockets[i] = 0;
    }
    // syslog(LOG_INFO, "Disconnected [fd: %i, ip: %s, port: %i]", connection, inet_ntoa(m_address.sin_addr), ntohs(m_address.sin_port));
    syslog(LOG_INFO, "Disconnected %s", &GetSocketInfo(connection)[0]);
}
string ServerSocket::GetSocketInfo(const int& connection){
    if (!IsSocketExists(connection))
        return "[SOCKET_ERROR]";
    getpeername(connection, (sockaddr*)&m_address, &m_addrlen);
    return "[fd: " + to_string(connection) + ", ip: " + inet_ntoa(m_address.sin_addr) + ", port: " + to_string(ntohs(m_address.sin_port)) + "]";
}
bool ServerSocket::IsSocketExists(const int& connection){
    for (int i = 0; i < MAX_SOCKETS; i++){
        if (m_sockets[i] == connection)
            return true;
    }
    return false;
}