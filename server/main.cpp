#ifdef DEBUG
#define _LOGMASK LOG_UPTO (LOG_DEBUG)
#else
#define _LOGMASK LOG_UPTO (LOG_INFO)
#endif

#include <syslog.h>

#include "signalcallbacks.h"
#include "configloader.h"
#include "serversettings.h"
#include "serversocket.h"
#include "commandhandler.h"
#include "commandfunctions.h"
#include "serializer.h"

#include "endpointconnection.h"
// #include "endpointsettings.h"
#include "endpointmanager.h"

// For testing
#include <chrono>
#include <thread>
#include <iostream>

const int LOGMASK = _LOGMASK;
const char* DAEMONNAME = "tmsd";

static bool s_termination = false;

CommandHandler* m_cmd;
ServerSocket* m_srv;
ServerSettings m_settings;
EndpointManager* m_EndpointManager;

using namespace std;

void LoadServerConfig();
void InitializeCommands();

int main(int argc, char* argv[]){
    // Open syslog
    openlog(DAEMONNAME, LOG_CONS | LOG_PID, LOG_USER);
    setlogmask(LOGMASK);
    syslog(LOG_INFO, "Starting server daemon");
    // Create signal handles
    SignalCallbacks::SetupCallbacks(&s_termination);
    // Initialize EndpointManager
    m_EndpointManager = new EndpointManager();
    // Load config
    LoadServerConfig();
    // Initialize commands
    m_cmd = new CommandHandler;
    InitializeCommands();
    // Create ServerSocket
    m_srv = new ServerSocket(&s_termination, m_settings.listeningPort);
    // FIXME test
    /*EndpointSettings est(m_endpoints[0].settings.name, true, "/mnt/images", 15);
    m_endpoints[0].settings = est;
    est = EndpointSettings(m_endpoints[1].settings.name, false, "/mnt/server/obrazki/telewizory/", 32);
    m_endpoints[1].settings = est;*/
    /*Serializer s;
    s.AddValue(est.name);
    s.AddValue(est.localcfg);
    s.AddValue(est.dir);
    s.AddValue(est.showtime);
    s.Serialize();
    string hs1 = s.DeserializeNext();
    bool hb1 = s.DeserializeNext<bool>();
    string hs2 = s.DeserializeNext();
    unsigned int hi1 = s.DeserializeNext<unsigned int>();
    // string h3 = s.DeserializeNext(); // It should throw and it throws
    syslog(LOG_DEBUG, "%s %b %s %i", &hs1[0], hb1, &hs2[0], hi1);*/
    // syslog(LOG_DEBUG, "%s", &h2[0]);
    // Main loop
    while (!s_termination)
    {
        m_srv->Handle([](char msg[], int n, int sd) -> string {
            string s(msg, n);
            if (m_cmd->Handle(s, sd)){
                return CommandHandler::CMD_VALID + m_cmd->GetOutput();
            }
            return CommandHandler::CMD_BAD;
        });
        this_thread::sleep_for(chrono::seconds(1)); // FIXME time to change
    }
    //delete l1;
    SignalCallbacks::RevertCallbacks();
    delete m_srv;
    delete m_cmd;
    delete m_EndpointManager;
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
    // Endpoint list
    vector<Config>* epList = cfgl.GetList("Endpoints");
    m_EndpointManager->LoadConnectionData(epList);
    /*EndpointConnection buffer;
    for (Config x : *epList){
        if (x.GetProperty<string>("Name", buffer.settings.name) &&
            x.GetProperty<string>("Ip", buffer.ip) &&
            x.GetProperty<unsigned short>("Port", buffer.port)){
            syslog(LOG_DEBUG, "Successfully loaded EndpointConnection");
            m_endpoints.push_back(buffer);
        } else{
            syslog(LOG_DEBUG, "Failure on loading EndpointConnection");
        }
    }*/
    delete epList;
}
void InitializeCommands(){
    m_cmd->AddCommand("TEST", Command{2, CommandFunctions::cmdtest});
    m_cmd->AddCommand("REV", Command{1, CommandFunctions::rtest});
    m_cmd->AddCommand("DISCON", Command{0, CommandFunctions::disconnect});
    m_cmd->AddCommand("GETEPSET", Command{1, CommandFunctions::getEndpointSettingsByName});
    m_cmd->AddCommand("GETEPLS", Command{0, CommandFunctions::getEndpointList});
}