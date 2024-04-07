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
const char* CONFIG_PATH = "/home/_vbq/cpp/tv-managing-system/server/";
// const char* CONFIG_PATH = "/home/_vbq/Desktop/cpp/tv-managing-system/server/"; // "/home/_vbq/cpp/tv-managing-system/server/"

static bool s_termination = false;

CommandHandler* m_CommandHandler;
ServerSocket* m_ServerSocket;
ServerSettings m_ServerSettings;
EndpointManager* m_EndpointManager;

using namespace std;

void LoadServerConfig();
void InitializeCommands();
void CleanUp();

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
    // Load endpoints config
    try{
        m_EndpointManager->LoadSettingsData(CONFIG_PATH + (string)"test/");
    } catch (const char* ex){
        syslog(LOG_ERR, "[Exception] %s", ex);
        CleanUp();
        return 1;
    }
    // Initialize commands
    m_CommandHandler = new CommandHandler;
    InitializeCommands();
    // Create ServerSocket
    m_ServerSocket = new ServerSocket(&s_termination, m_ServerSettings.listeningPort);
    // Main loop
    while (!s_termination)
    {
        m_ServerSocket->Handle([](char msg[], int n, int sd) -> string {
            string s(msg, n);
            if (m_CommandHandler->Handle(s, sd)){
                return CommandHandler::CMD_VALID + m_CommandHandler->GetOutput();
            }
            return CommandHandler::CMD_BAD;
        });
        this_thread::sleep_for(chrono::seconds(1)); // FIXME time to change
    }
    CleanUp();
    syslog(LOG_INFO, "Daemon exited successfully");
    return 0;
}

void LoadServerConfig(){
    ConfigLoader cfgl = ConfigLoader(CONFIG_PATH + (string)"example_config.cfg");
    cfgl.Load();
    // Properties
    cfgl.GetProperty<string>("ListeningIp", m_ServerSettings.listeningIp);
    cfgl.GetProperty<unsigned short>("ListeningPort", m_ServerSettings.listeningPort);
    cfgl.GetProperty<string>("PasswordHash", m_ServerSettings.passwordHash);
    // Endpoint list
    vector<Config>* epList = cfgl.GetList("Endpoints");
    m_EndpointManager->LoadConnectionData(epList);
    delete epList;
}
void InitializeCommands(){
    m_CommandHandler->AddCommand("TEST", Command{2, CommandFunctions::cmdtest});
    m_CommandHandler->AddCommand("REV", Command{1, CommandFunctions::rtest});
    m_CommandHandler->AddCommand("DISCON", Command{0, CommandFunctions::disconnect});
    m_CommandHandler->AddCommand("GETEPSET", Command{1, CommandFunctions::getEndpointSettingsByName});
    m_CommandHandler->AddCommand("GETEPLS", Command{0, CommandFunctions::getEndpointList});
    m_CommandHandler->AddCommand("SETEPSET", Command{2, CommandFunctions::setEndpointSettings});
}
void CleanUp(){
    SignalCallbacks::RevertCallbacks();
    delete m_ServerSocket;
    delete m_CommandHandler;
    delete m_EndpointManager;
}