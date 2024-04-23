#ifdef DEBUG

#define _LOGMASK LOG_UPTO (LOG_DEBUG)
#define _CONFIG_PATH "/home/_vbq/cpp/tv-managing-system/server/test/"

#else

#define _LOGMASK LOG_UPTO (LOG_INFO)
#define _CONFIG_PATH "/etc/tmsd/"

#endif

#include <syslog.h>

#include "signalcallbacks.h"
#include "configloader.h"
#include "serversettings.h"
#include "serversocket.h"
#include "commandhandler.h"
#include "commandfunctions.h"
#include "authmanager.h"
// #include "actionqueue.h"

#include "serializer.h"
#include "endpointconnection.h"
#include "endpointmanager.h"

// For testing
#include <chrono>
#include <thread>

const int LOGMASK = _LOGMASK;
const char* DAEMONNAME = "tmsd";

/*
 * Config
*/
const char* CONFIG_PATH = _CONFIG_PATH;
const char* CONFIG_ENDPOINTS_DIR = "epconf/"; // Inside CONFIG_PATH
const char* CONFIG_SETTINGS_FILE = "settings.cfg"; // Inside CONFIG_PATH
const char* CONFIG_AUTHKEY_FILE = "authkey"; // Inside CONFIG_PATH

/*
 * Global loop
*/
static bool s_termination = false;

/*
 * Classes
*/
CommandHandler* m_CommandHandler;
ServerSocket* m_ServerSocket;
EndpointManager* m_EndpointManager;
AuthManager* m_AuthManager;
// ActionQueue* m_ActionQueue;

/*
 * Structs
*/
ServerSettings m_serversettings;

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
    m_EndpointManager = new EndpointManager((string)CONFIG_PATH + CONFIG_ENDPOINTS_DIR);
    //Initialize AuthManager
    m_AuthManager = new AuthManager((string)CONFIG_PATH + CONFIG_AUTHKEY_FILE);
    // Initialize ActionQueue
    // FIXME test of actionqueue
    /*m_ActionQueue = new ActionQueue();
    int* a = new int(7);
    m_ActionQueue->Add(a, false);
    m_ActionQueue->Handle();*/
    // Load config
    LoadServerConfig();
    // Load endpoints config
    try{
        m_EndpointManager->LoadSettingsData();
    } catch (const char* ex){
        syslog(LOG_ERR, "[Exception] %s", ex);
        CleanUp();
        return 1;
    }
    // Initialize commands
    m_CommandHandler = new CommandHandler;
    InitializeCommands();
    // Create ServerSocket
    m_ServerSocket = new ServerSocket(&s_termination, m_serversettings.listeningPort, m_serversettings.maxConnections);
    // Main loop
    while (!s_termination)
    {
        //m_ActionQueue->Handle();
        m_ServerSocket->Handle([](char msg[], int n, int sd) -> string {
            string s(msg, n);
            int h = m_CommandHandler->Handle(s, sd);
            if (h == CMD_H_VALID){
                return CommandHandler::CMD_VALID + m_CommandHandler->GetOutput() + "\r\n";
            }
            else if (h == CMD_H_AUTHERR){
                return ServerSocket::SMSG_AUTH_REQ;
            }
            return CommandHandler::CMD_BAD;
        });
        m_EndpointManager->SaveSettings();
        m_AuthManager->Handle();
        this_thread::sleep_for(chrono::milliseconds(250)); // FIXME time to change
    }
    CleanUp();
    syslog(LOG_INFO, "Daemon exited successfully");
    return 0;
}

void LoadServerConfig(){
    ConfigLoader cfgl = ConfigLoader((string)CONFIG_PATH + CONFIG_SETTINGS_FILE);
    cfgl.Load();
    // Properties
    cfgl.GetProperty<string>("ListeningIp", m_serversettings.listeningIp);
    cfgl.GetProperty<unsigned short>("ListeningPort", m_serversettings.listeningPort);
    cfgl.GetProperty<string>("PasswordHash", m_serversettings.passwordHash);
    cfgl.GetProperty<int>("MaxConnections", m_serversettings.maxConnections);
    // Endpoint list
    vector<Config>* epList = cfgl.GetList("Endpoints");
    m_EndpointManager->LoadConnectionData(epList);
    delete epList;
}
void InitializeCommands(){
    m_CommandHandler->AddCommand("HELLO", Command{0, CommandFunctions::hello, false});
    m_CommandHandler->AddCommand("REV", Command{1, CommandFunctions::rtest, true});
    m_CommandHandler->AddCommand("DISCON", Command{0, CommandFunctions::disconnect, true});
    m_CommandHandler->AddCommand("GETEPSET", Command{1, CommandFunctions::getEndpointSettingsByName, true});
    m_CommandHandler->AddCommand("GETEPLS", Command{0, CommandFunctions::getEndpointList, true});
    m_CommandHandler->AddCommand("SETEPSET", Command{2, CommandFunctions::setEndpointSettings, true});
}
void CleanUp(){
    SignalCallbacks::RevertCallbacks();
    delete m_ServerSocket;
    delete m_CommandHandler;
    delete m_EndpointManager;
}