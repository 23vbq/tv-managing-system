#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#define _READBUFF_LEN 1024

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
    static const string SMSG_HELLO; // Hello message
    static const string SMSG_REFNRE; // Connection refused w/o reason
    static const string SMSG_CLSD; // Connection closed
    static const string SMSG_AUTH_REQ; // Require auth
    static const string SOCKET_LOOP_IGNORE_SIG; // Signal to ignore sending data

private:
    bool* s_termination;
    int MAX_SOCKETS; // Number of max connected clients

    int m_server_fd;
    sockaddr_in m_address;
    socklen_t m_addrlen;
    int m_opt = 1;

    fd_set m_readfds;
    int* m_sockets;
    char m_readbuff[_READBUFF_LEN];

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
     * @param maxConnections number of max simultaneously connections
    */
    ServerSocket(bool*, uint16_t, int);
    ~ServerSocket();

    /**
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
    /**
     * Gets info about connected client
     * @param connection FD of connection
     * @return [fd: %i, ip: %s, port: %i]
    */
    string GetSocketInfo(const int&);
    /**
     * Checks if connection exists
     * @param connection FD of connection
    */
    bool IsSocketExists(const int&);
};

#endif