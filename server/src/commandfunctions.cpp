#include "commandfunctions.h"
#include "serializer.h"
// FIXME to remove on move to EndpointManager
#include "endpointconnection.h"

extern CommandHandler* m_cmd;
extern ServerSocket* m_srv;
extern vector<EndpointConnection> m_endpoints;

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
        for (const EndpointConnection& ep : m_endpoints){
            if (ep.settings.name == *name){
                Serializer sr;
                sr.AddValue(ep.settings.name);
                sr.AddValue(ep.settings.localcfg);
                sr.AddValue(ep.settings.dir);
                sr.AddValue(ep.settings.showtime);
                output = sr.Serialize();
                return;
            }
        }
    }
    void getEndpointList(vector<string> x, string& output){
        output = "";
        Serializer sr;
        sr.AddValue<size_t>(m_endpoints.size());
        for (const EndpointConnection& ep : m_endpoints){
            sr.AddValue(ep.settings.name);
        }
        output = sr.Serialize();
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