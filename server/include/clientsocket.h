#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#define _READBUFF_LEN 1024

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <syslog.h>

using namespace std;

class ClientSocket
{
private:
    int m_client_fd; // Client socket FD
    sockaddr_in m_address; // Address of connection
    socklen_t m_addrlen; // Length of address
    bool isConnected; // Is connected to server

    char m_readbuff[_READBUFF_LEN]; // Buffer for reading from socket
    size_t m_readsize; // Length of read data

    /**
     * Creates socket
    */
    void CreateSocket();
    /**
     * Closes socket
    */
    void CloseSocket();
public:
    ClientSocket();
    ~ClientSocket();

    /**
     * Connects to server
     * @param address address of server
     * @param port port of server
     * @return True if connection was successfull
    */
    bool Connect(const string&, const uint16_t&);
    /**
     * Sends data to server
     * @param message data to send
     * @return True if data was sent
    */
    bool Send(const string&);
    /**
     * Reads data from socket
     * @param result variable to store read data
     * @return True if successfully read data
    */
    bool Read(string*);
    /**
     * Sends disconnect message to server and closes socket
    */
    void Disconnect();
    /**
     * Returns isConnected status
    */
    bool IsConnected();
};

#endif