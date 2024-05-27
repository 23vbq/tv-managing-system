#ifdef DEBUG

#define _LOGMASK LOG_UPTO (LOG_DEBUG)
#define _CONFIG_PATH "/home/_vbq/cpp/tv-managing-system/endpoint/test/"

#else

#define _LOGMASK LOG_UPTO (LOG_INFO)
#define _CONFIG_PATH "/etc/tmse/"

#endif

#include <syslog.h>
#include <stdio.h>

#include "signalcallbacks.h"
#include "endpointserversettings.h"
#include "commandfunctions.h"
#include "settingsmanager.h"
#include "slideshowmanager.h"
#include "windowmanager.h"

#include "configloader.h"
#include "serversocket.h"
#include "commandhandler.h"

// FIXME is this needed
#include <chrono>
#include <thread>

const int LOGMASK = _LOGMASK;
const char* PROCNAME = "tmse";

/*
 * Config
*/
const char* CONFIG_PATH = _CONFIG_PATH;
const char* CONFIG_SETTINGS_FILE = "serversocket.cfg"; // Inside CONFIG_PATH
const char* CONFIG_AUTHKEY_FILE = "authkey"; // Inside CONFIG_PATH

/*
 * Global loop
*/
static bool s_termination = false;

/*
 * Classes
*/
ServerSocket* m_ServerSocket;
CommandHandler* m_CommandHandler;
AuthManager* m_AuthManager;

SettingsManager* m_SettingsManager;

WindowManager* m_WindowManager;
SlideshowManager* m_SlideshowManager;

/*
 * Structs
*/
EndpointServerSettings m_endpointserversettings;

using namespace std;

void LoadServerConfig();
void ServerLoopThread();
void InitializeCommands();
void CleanUp();

int main(int argc, char* argv[]){
    openlog(PROCNAME, LOG_CONS | LOG_PID, LOG_USER);
    setlogmask(LOGMASK);
    syslog(LOG_INFO, "Starting endpoint");

    // Create signal handles
    SignalCallbacks::SetupCallbacks(&s_termination);

    // Initialize WM
    try{
        m_WindowManager = new WindowManager();
        m_WindowManager->SetPtrSTermination(&s_termination);
    } catch(const char *e){
        syslog(LOG_ERR, "[WindowManager] %s", e);
        printf("[WindowManager] %s\n", e);
        CleanUp();
        exit(1);
    }

    // Setup WM thread
    thread wm_thread(&WindowManager::Run, m_WindowManager);
    wm_thread.detach();

    // Timer
    time_t timer_now;
    time_t timer_last;
    time_t timer_delta;
    time_t timer_next_window = 10;

    // FIXME slideshow test
    m_SlideshowManager = new SlideshowManager();
    m_SlideshowManager->GetFilesInPath("/home/_vbq");
    m_SlideshowManager->OpenAllImages();

    // Load config
    LoadServerConfig();
    // Initialize AuthManager
    try{
        m_AuthManager = new AuthManager((string)CONFIG_PATH + CONFIG_AUTHKEY_FILE);
    } catch (const char * e){
        syslog(LOG_ERR, "[AuthManager] %s", e);
        CleanUp();
        exit(1);
    }
    // Initialize settings manager
    m_SettingsManager = new SettingsManager();

    // Initialize commands
    m_CommandHandler = new CommandHandler;
    InitializeCommands();
    // Initialize server socket
    m_ServerSocket = new ServerSocket(&s_termination, m_endpointserversettings.listeningPort, 1);
    thread srv_thread(ServerLoopThread);
    srv_thread.detach();
    
    // Timer loop
    timer_last = time(NULL);
    while(!s_termination){
        timer_now = time(NULL);
        timer_delta = timer_now - timer_last;
        timer_next_window -= timer_delta;
        if (timer_next_window <= 0){
            m_WindowManager->NextWindow();
            timer_next_window = 10;
        }
        timer_last = timer_now;
        this_thread::sleep_for(chrono::milliseconds(250)); // FIXME time to change
    }

    // Stop WM
    m_WindowManager->StopEventLoop();
    if (wm_thread.joinable())
        wm_thread.join();

    // Wait for server loop stop
    if (srv_thread.joinable())
        srv_thread.join();

    CleanUp();
    return 0;
}
void LoadServerConfig(){
    ConfigLoader cfgl = ConfigLoader((string) CONFIG_PATH + CONFIG_SETTINGS_FILE);
    cfgl.Load();
    cfgl.GetProperty<unsigned short>("ListeningPort", m_endpointserversettings.listeningPort);
}
void ServerLoopThread(){
    // Main loop
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
        this_thread::sleep_for(chrono::milliseconds(250)); // FIXME time to change
    }
}
void InitializeCommands(){
    m_CommandHandler->AddCommand("HELLO", Command{0, CommandFunctions::hello, false});
    m_CommandHandler->AddCommand("DISCON", Command{0, CommandFunctions::disconnect, true});
    m_CommandHandler->AddCommand("SETEPSET", Command{1, CommandFunctions::setEndpointSettings, true});
    //m_CommandHandler->AddCommand("AUTHK", Command{1, CommandFunctions::authKey, false});
    m_CommandHandler->AddCommand("PING", Command{0, CommandFunctions::ping, false});
}
void CleanUp(){
    SignalCallbacks::RevertCallbacks();
    delete m_SlideshowManager;
    delete m_WindowManager;

    delete m_ServerSocket;
    delete m_CommandHandler;
    delete m_SettingsManager;
}