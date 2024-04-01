#include "clientsocketqt.h"

// Constructors

ClientSocketQt::ClientSocketQt(QObject* parent){
    m_socket = new QTcpSocket(parent);
}

ClientSocketQt::~ClientSocketQt(){
    delete m_socket;
}

// Public functions

bool ClientSocketQt::Connect(const std::string& address, const unsigned int& port){
    m_socket->connectToHost(QString::fromStdString(address), port);
    if (m_socket->waitForConnected(CONNECTION_TIMEOUT)){
        m_readbuff = m_socket->readAll();
        m_connected = true;
        return m_connected;
    }
    m_connected = false;
    return m_connected;
}
bool ClientSocketQt::IsConnected(){
    return m_connected;
}
