#include "clientsocketqt.h"

// Constructors

ClientSocketQt::ClientSocketQt(QObject* parent){
    m_socket = new QTcpSocket(parent);
}

ClientSocketQt::~ClientSocketQt(){
    Disconnect();
    if (m_socket->state() == QAbstractSocket::UnconnectedState ||
        m_socket->waitForDisconnected(CONNECTION_TIMEOUT))
        delete m_socket;
}

// Public functions

bool ClientSocketQt::Connect(const std::string& address, const unsigned int& port){
    m_socket->connectToHost(QString::fromStdString(address), port);
    if (m_socket->waitForConnected(CONNECTION_TIMEOUT)){
        m_readbuff = m_socket->readAll();
        return true;
    }
    return false;
}
void ClientSocketQt::Disconnect(){
    if (!IsConnected())
        return;
    Send("DISCON");
    m_socket->disconnectFromHost();
}
bool ClientSocketQt::Send(std::string message){
    qint64 sendlen;
    size_t n = message.length();
    sendlen = m_socket->write(&message[0], n);
    return sendlen == n;
}
bool ClientSocketQt::IsConnected(){
    return m_socket->state() == QAbstractSocket::ConnectedState;
}
