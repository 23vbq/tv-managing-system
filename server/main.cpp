#include <syslog.h>

#include "signalcallbacks.h"
#include "configloader.h"
#include "serversettings.h"
#include "endpointconnection.h"

// For testing
#include <chrono>
#include <thread>
#include <iostream>

const int LOGMASK = LOG_UPTO (LOG_DEBUG);
const char* DAEMONNAME = "tmsd";

static bool s_mainloop = true;

ServerSettings m_settings;
vector<EndpointConnection> m_endpoints;

using namespace std;

void LoadServerConfig();

int main(int argc, char* argv[]){
    // Open syslog
    openlog(DAEMONNAME, LOG_CONS | LOG_PID, LOG_USER);
    setlogmask(LOGMASK);
    syslog(LOG_INFO, "Starting server daemon");
    // Create signal handles
    SignalCallbacks::SetupCallbacks(&s_mainloop);
    // Load config
    LoadServerConfig();
    // FIXME tests
    int x = 0, x2 = 0;
    // bool b1 = cfgl.GetProperty<int>("liczba", x);
    // bool b2 = cfgl.GetProperty<int>("testing", x);
    // vector<Config>* l1 = cfgl.GetList("lista");
    // for (int i = 0; i < l1->size(); i++)
    //     syslog(LOG_CRIT, "%s", &(l1->at(i).ToString())[0]);
    // bool b3 = l1->at(0).GetProperty<int>("fajne", x2);
    while (s_mainloop)
    {
        syslog(LOG_NOTICE, "Test message");
        this_thread::sleep_for(chrono::seconds(10));
    }
    //delete l1;
    syslog(LOG_INFO, "Daemon exited successfully");
    return 0;
}

void LoadServerConfig(){
    ConfigLoader cfgl = ConfigLoader("/home/_vbq/cpp/tv-managing-system/server/example_config.cfg");
    cfgl.Load();
    // Properties
    cfgl.GetProperty<string>("ListeningIp", m_settings.listeningIp);
    cfgl.GetProperty<unsigned short>("ListeningPort", m_settings.listeningPort);
    cfgl.GetProperty<string>("PasswordHash", m_settings.passwordHash);
    // List
    vector<Config>* epList = cfgl.GetList("Endpoints");
    EndpointConnection buffer;
    /*int nEpList = epList->size();
    for (int i = 0; i < nEpList; i++){
        if (epList->at(i))
    }*/
    for (Config x : *epList){
        if (x.GetProperty<string>("Name", buffer.name) &&
            x.GetProperty<string>("Ip", buffer.ip) &&
            x.GetProperty<unsigned short>("Port", buffer.port)){
            syslog(LOG_DEBUG, "Successfully loaded EndpointConnection");
            m_endpoints.push_back(buffer);
        } else{
            syslog(LOG_DEBUG, "Failure on loading EndpointConnection");
        }
    }
    delete epList;
}