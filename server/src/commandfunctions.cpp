#include "commandfunctions.h"
#include "serializer.h"
// FIXME to remove on move to EndpointManager
#include "endpointmanager.h"

extern CommandHandler* m_cmd;
extern ServerSocket* m_srv;
extern EndpointManager* m_EndpointManager;

namespace CommandFunctions{
    void disconnect(vector<string> x, string& output){
        syslog(LOG_INFO, "Client requested disconnection");
        int sd = m_cmd->GetCurrentSd();
        m_srv->Disconnect(sd, "");
        output = ServerSocket::SOCKET_LOOP_IGNORE_SIG;
    }
    void getEndpointSettingsByName(vector<string> x, string& output){
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
        delete ep;
    }
    void getEndpointList(vector<string> x, string& output){
        output = "";
        Serializer sr;
        vector<string>* names = m_EndpointManager->GetNames();
        sr.AddList(*names);
        /*sr.AddValue<size_t>(names->size());
        for (const string& x : *names){
            sr.AddValue(x);
        }*/
        output = sr.Serialize();
        delete names;
    }
    void rtest(vector<string> x, string& output){
        size_t n = x[0].length();
        output = "";
        for (int i = n - 2; i >= 0; i--)
            output += x[0][i];
        output += '\n';
    }
    void cmdtest(vector<string> x, string& output){
        syslog(LOG_ALERT, "Function said: %s %i", &(x[0])[0], stoi(x[1]));
        output = "fajne";
    }
}