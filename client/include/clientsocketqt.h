#ifndef CLIENT_SOCKET_QT_H
#define CLIENT_SOCKET_QT_H

#include <QTcpSocket>

#define CONNECTION_TIMEOUT 5000

class ClientSocketQt{
private:
    QTcpSocket* m_socket;

    QByteArray m_readbuff;
public:
    ClientSocketQt(QObject* parent = nullptr);
    ~ClientSocketQt();

    bool Connect(const std::string&, const unsigned int&);
    void Disconnect();
    bool Send(std::string);
    void Read(std::string&);

    bool IsConnected();
};

#endif
