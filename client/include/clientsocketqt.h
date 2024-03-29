#ifndef CLIENT_SOCKET_QT_H
#define CLIENT_SOCKET_QT_H

#include <QTcpSocket>

class ClientSocketQt{
private:
    QTcpSocket m_socket;

    QByteArray m_readbuff;
public:
    ClientSocketQt(QObject*);
    ~ClientSocketQt();

    void Send(std::string);
    void Read(std::string&);
};

#endif