#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <syslog.h>

using namespace std;

class ServerSocket
{
public:
    static const int MAX_SOCKETS = 2; // Number of max connected clients
    static const string SMSG_HELLO; // Hello message
    static const string SMSG_REFNRE; // Connection refused w/o reason
    static const string SMSG_CLSD; // Connection closed
    static const string SOCKET_LOOP_IGNORE_SIG;

private:
    bool* s_termination;

    int m_server_fd;
    sockaddr_in m_address;
    socklen_t m_addrlen;
    int m_opt = 1;

    fd_set m_readfds;
    int m_sockets[MAX_SOCKETS];

    /**
     * Accepts new connections
     * @return TRUE if new connection was handled, FALSE if no new connection
    */
    bool AcceptConnectionHandle();
    /**
     * Handles lost connection with client
     * @param connection FD of connection
    */
    void LostConnectionHandle(int);
public:
    /**
     * @param termination bool* to variable indicating termination of program
     * @param port number of port to listen
    */
    ServerSocket(bool*, uint16_t);
    ~ServerSocket();

    /***
     * Handles ServerSocket work. Should be run in loop.
     * @param responseCall function returning string to be sent as response
     *      for char[] message with length n from client and sd socket descriptor
    */
    void Handle(string (*)(char[], int, int));
    /**
     * Disconnects client with reason message
     * @param connection FD of connection
     * @param reason Message to be sent to client, if string is empty message will not be sent
    */
    void Disconnect(int, string);
};

#endif