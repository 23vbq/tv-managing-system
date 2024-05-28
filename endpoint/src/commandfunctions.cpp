#include "commandfunctions.h"
#include "serializer.h"
#include "slideshowmanager.h"

extern CommandHandler* m_CommandHandler;
extern ServerSocket* m_ServerSocket;
extern SettingsManager* m_SettingsManager;
extern SlideshowManager* m_SlideshowManager;

namespace CommandFunctions{
    void disconnect(CF_ARGS){
        syslog(LOG_INFO, "Client requested disconnection");
        int sd = m_CommandHandler->GetCurrentSd();
        m_ServerSocket->Disconnect(sd, "");
        output = ServerSocket::SOCKET_LOOP_IGNORE_SIG;
    }
    void setEndpointSettings(CF_ARGS){
        // Buffer new data
        EndpointSettings buff;
        Serializer sr(x[0]);
        buff.name = sr.DeserializeNext();
        buff.localcfg = sr.DeserializeNext<bool>();
        buff.dir = sr.DeserializeNext();
        buff.showtime = sr.DeserializeNext<unsigned int>();

        EndpointSettings current;
        bool reloadImg = (buff.dir != current.dir);

        // Set settings
        m_SettingsManager->SetSettings(buff);

        // Handle
        if (reloadImg)
            m_SlideshowManager->ReloadImages(buff.dir);
    }
    void hello(CF_ARGS){
        output = ServerSocket::SMSG_HELLO;
    }
    void ping(CF_ARGS){
        output = to_string(time(NULL));
    }
}