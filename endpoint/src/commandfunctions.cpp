#include "commandfunctions.h"
#include "serializer.h"
#include "slideshowmanager.h"
#include "authmanager.h"

extern CommandHandler* m_CommandHandler;
extern ServerSocket* m_ServerSocket;
extern SettingsManager* m_SettingsManager;
extern SlideshowManager* m_SlideshowManager;
extern AuthManager* m_AuthManager;

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
    void authKey(CF_ARGS){
        int sd = m_CommandHandler->GetCurrentSd();
        string sock_info = m_ServerSocket->GetSocketInfo(sd);
        syslog(LOG_AUTH, "Client auth %s", &sock_info[0]);
        if (m_AuthManager->Auth(sd, x[0])){
            output = "SUCCESS";
            syslog(LOG_AUTH, "Successfully authenticated %s", &sock_info[0]);
        } else{
            output = "FAIL";
            syslog(LOG_AUTH, "Failure authentication %s", &sock_info[0]);
        }
    }
    void hello(CF_ARGS){
        output = ServerSocket::SMSG_HELLO;
    }
    void ping(CF_ARGS){
        output = to_string(time(NULL));
    }
}