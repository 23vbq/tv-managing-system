#include "commandfunctions.h"
#include "serializer.h"
// FIXME to remove on move to EndpointManager
#include "endpointmanager.h"

extern CommandHandler* m_CommandHandler;
extern ServerSocket* m_ServerSocket;
extern EndpointManager* m_EndpointManager;
extern AuthManager* m_AuthManager;

namespace CommandFunctions{
    void disconnect(CF_ARGS){
        syslog(LOG_INFO, "Client requested disconnection");
        int sd = m_CommandHandler->GetCurrentSd();
        m_ServerSocket->Disconnect(sd, "");
        output = ServerSocket::SOCKET_LOOP_IGNORE_SIG;
    }
    void getEndpointSettingsByName(CF_ARGS){
        output = "";
        string* name = &x[0];
        EndpointSettings* ep = m_EndpointManager->GetSettings(*name);
        if (ep == NULL)
            return;
        Serializer sr;
        sr.AddValue(ep->name);
        sr.AddValue(ep->localcfg);
        sr.AddValue(ep->dir);
        sr.AddValue(ep->showtime);
        output = sr.Serialize();
        // delete ep;
    }
    void getEndpointList(CF_ARGS){
        output = "";
        Serializer sr;
        vector<string>* names = m_EndpointManager->GetNames();
        sr.AddList(*names);
        output = sr.Serialize();
        delete names;
    }
    void setEndpointSettings(CF_ARGS){
        output = "";
        EndpointSettings* ep = m_EndpointManager->GetSettings(x[0]);
        if (ep == NULL){
            syslog(LOG_ERR, "Requested SetSettings for invalid endpoint name: %s", &(x[0])[0]);
            output = "Invalid endpoint name";
            return;
        }
        Serializer sr(x[1]);
        EndpointSettings newSettings{sr.DeserializeNext(), sr.DeserializeNext<bool>(), sr.DeserializeNext(), sr.DeserializeNext<unsigned int>()};
        m_EndpointManager->SetSettings(ep, newSettings);
    }
    void ping(CF_ARGS){
        output = "";
        if (!m_EndpointManager->SendToOne(x[0], "PING", &output)){
            output = "BAD";
        }
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
}