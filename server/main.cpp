#ifdef DEBUG
#define _LOGMASK LOG_UPTO (LOG_DEBUG)
#else
#define _LOGMASK LOG_UPTO (LOG_INFO)
#endif

#include <syslog.h>

#include "signalcallbacks.h"
#include "configloader.h"
#include "serversettings.h"
#include "endpointconnection.h"
#include "endpointsettings.h"
#include "serversocket.h"
#include "commandhandler.h"
#include "commandfunctions.h"
#include "serializer.h"

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
vector<EndpointConnection> m_endpoints;

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
    // Load config
    LoadServerConfig();
    // Initialize commands
    m_cmd = new CommandHandler;
    InitializeCommands();
    // Create ServerSocket
    m_srv = new ServerSocket(&s_termination, m_settings.listeningPort);
    // FIXME test
    EndpointSettings est {"Test", true, "/mnt/images", 15};
    Serializer s;
    s.AddValue(est.name);
    // s.AddValue(est.localcfg);
    s.AddValue(est.dir);
    // s.AddValue(est.showtime);
    s.Serialize();
    string h1 = s.DeserializeNext();
    string h2 = s.DeserializeNext();
    // string h3 = s.DeserializeNext(); // It should throw and it throws
    syslog(LOG_DEBUG, "%s", &h1[0]);
    syslog(LOG_DEBUG, "%s", &h2[0]);
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
    EndpointConnection buffer;
    for (Config x : *epList){
        if (x.GetProperty<string>("Name", buffer.settings.name) &&
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
void InitializeCommands(){
    m_cmd->AddCommand("TEST", Command{2, CommandFunctions::cmdtest});
    m_cmd->AddCommand("REV", Command{1, CommandFunctions::rtest});
    m_cmd->AddCommand("DISCON", Command{0, CommandFunctions::disconnect});
}