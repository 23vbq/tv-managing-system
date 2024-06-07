#ifdef DEBUG

#define _LOGMASK LOG_UPTO (LOG_DEBUG)
#define _CONFIG_PATH "/home/_vbq/cpp/tv-managing-system/server/test/"

#else

#define _LOGMASK LOG_UPTO (LOG_INFO)
#define _CONFIG_PATH "/etc/tmsd/"

#endif

#define LT_SRVHND chrono::milliseconds(100)
#define LT_MAINLOOP chrono::seconds(5)

#include <syslog.h>

#include "signalcallbacks.h"
#include "serversettings.h"
#include "commandfunctions.h"
#include "authmanager.h"
#include "endpointmanager.h"
// #include "actionqueue.h"

#include "serializer.h"
#include "serversocket.h"
#include "commandhandler.h"
#include "configloader.h"
#include "endpointconnection.h"

// FIXME is this needed
#include <chrono>
#include <thread>

// FIXME test
#include "clientsocket.h"

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
void ServerLoopThread();
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
    try{
        m_AuthManager = new AuthManager((string)CONFIG_PATH + CONFIG_AUTHKEY_FILE);
    } catch (const char * e){
        syslog(LOG_ERR, "[AuthManager] %s", e);
        CleanUp();
        exit(1);
    }

    // Load config
    LoadServerConfig();
    // Load endpoints config
    try{
        m_EndpointManager->LoadSettingsData();
        /*m_EndpointManager->InitializeEndpointSockets();
        m_EndpointManager->SendUpdate();*/
    } catch (const char* ex){
        syslog(LOG_ERR, "[EndpointManager] %s", ex);
        CleanUp();
        return 1;
    }
    // Initialize commands
    m_CommandHandler = new CommandHandler;
    InitializeCommands();
    // Create ServerSocket
    m_ServerSocket = new ServerSocket(&s_termination, m_serversettings.listeningPort, m_serversettings.maxConnections);

    syslog(LOG_INFO, "Initialization completed successfully");

    thread srv_thread(ServerLoopThread);
    srv_thread.detach();

    // Main loop
    while (!s_termination)
    {
        m_EndpointManager->SaveSettings();
        m_EndpointManager->InitializeEndpointSockets();
        m_EndpointManager->SendUpdate();
        m_AuthManager->Handle();
        this_thread::sleep_for(LT_MAINLOOP); // FIXME time to change
    }

    // Stop server loop
    if (srv_thread.joinable())
        srv_thread.join();

    // Program end
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
void ServerLoopThread(){
    while(!s_termination){
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
        this_thread::sleep_for(LT_SRVHND);
    }
}
void InitializeCommands(){
    m_CommandHandler->AddCommand("HELLO", Command{0, CommandFunctions::hello, false});
    m_CommandHandler->AddCommand("DISCON", Command{0, CommandFunctions::disconnect, true});
    m_CommandHandler->AddCommand("GETEPSET", Command{1, CommandFunctions::getEndpointSettingsByName, true});
    m_CommandHandler->AddCommand("GETEPLS", Command{0, CommandFunctions::getEndpointList, true});
    m_CommandHandler->AddCommand("SETEPSET", Command{2, CommandFunctions::setEndpointSettings, true});
    m_CommandHandler->AddCommand("PING", Command{1, CommandFunctions::ping, true});
    m_CommandHandler->AddCommand("AUTHK", Command{1, CommandFunctions::authKey, false});
}
void CleanUp(){
    SignalCallbacks::RevertCallbacks();
    delete m_ServerSocket;
    delete m_CommandHandler;
    delete m_EndpointManager;
    delete m_AuthManager;
}