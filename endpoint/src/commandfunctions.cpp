#include "commandfunctions.h"
#include "serializer.h"

extern CommandHandler* m_CommandHandler;
extern ServerSocket* m_ServerSocket;
extern SettingsManager* m_SettingsManager;

namespace CommandFunctions{
    void disconnect(CF_ARGS){
        syslog(LOG_INFO, "Client requested disconnection");
        int sd = m_CommandHandler->GetCurrentSd();
        m_ServerSocket->Disconnect(sd, "");
        output = ServerSocket::SOCKET_LOOP_IGNORE_SIG;
    }
    void setEndpointSettings(CF_ARGS){
        EndpointSettings buff;
        Serializer sr(x[0]);
        buff.name = sr.DeserializeNext();
        buff.localcfg = sr.DeserializeNext<bool>();
        buff.dir = sr.DeserializeNext();
        buff.showtime = sr.DeserializeNext<unsigned int>();
        m_SettingsManager->SetSettings(buff);
    }
    void hello(CF_ARGS){
        output = ServerSocket::SMSG_HELLO;
    }
    void ping(CF_ARGS){
        output = to_string(time(NULL));
    }
}