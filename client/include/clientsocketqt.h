#ifndef CLIENT_SOCKET_QT_H
#define CLIENT_SOCKET_QT_H

#include <QTcpSocket>
#include <QMessageBox>

#include "authwindow.h"

#define CONNECTION_TIMEOUT 5000

class ClientSocketQt{
private:
    QTcpSocket* m_socket; // Pointer to Tcp Socket

    QByteArray m_readbuff; // Buffer to store read byte output
    QString m_readres; // Buffer to store read output (converted from bytes)

    AuthWindow* aw;
public:
    /**
     * Constructor of client socket
     * @param parent pointer to QObject parent
     */
    ClientSocketQt(QObject* parent = nullptr);
    /**
     * Destructor
     */
    ~ClientSocketQt();

    /**
     * Connects to server
     * @param address address of server
     * @param prot listening port of server
     * @return True if connected
     */
    bool Connect(const std::string&, const unsigned int&);
    /**
     * Disconnects from connected server
     */
    void Disconnect();
    /**
     * Sends message to server. Waits for data to be wirtten (waits for CONNECTION_TIMEOUT ms).
     * @param message string data to be sent
     * @return True if message was sent, and has same length as orginal data
     */
    bool Send(std::string);
    /**
     * Reads data from server. Waits for data from server for CONNECTION_TIMEOUT ms.
     * @param result reference to string to store output
     * @return Length of read data
     */
    qint64 Read(std::string&);

    /**
     * Checks for is connected to server
     */
    bool IsConnected();

    void SetAwPtr(AuthWindow*);
};

#endif
