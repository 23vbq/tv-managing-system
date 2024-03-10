#include <syslog.h>

#include "signalcallbacks.h"
#include "configloader.h"
#include "serversettings.h"
#include "endpointconnection.h"
#include "serversocket.h"
#include "commandhandler.h"

// For testing
#include <chrono>
#include <thread>
#include <iostream>

const int LOGMASK = LOG_UPTO (LOG_DEBUG);
const char* DAEMONNAME = "tmsd";

static bool s_termination = false;

ServerSocket* m_srv;
ServerSettings m_settings;
vector<EndpointConnection> m_endpoints;

using namespace std;

void LoadServerConfig();
// FIXME testing functions
string rtest(char[], int);
void cmdtest();

int main(int argc, char* argv[]){
    // Open syslog
    openlog(DAEMONNAME, LOG_CONS | LOG_PID, LOG_USER);
    setlogmask(LOGMASK);
    syslog(LOG_INFO, "Starting server daemon");
    // Create signal handles
    SignalCallbacks::SetupCallbacks(&s_termination);
    // Load config
    LoadServerConfig();
    // FIXME tests
    CommandHandler cmd;
    cmd.AddCommand("test", cmdtest);
    cmd.Handle("test");
    // Create ServerSocket
    m_srv = new ServerSocket(&s_termination, m_settings.listeningPort);
    // Main loop
    while (!s_termination)
    {
        //syslog(LOG_NOTICE, "Test message");
        m_srv->Handle(rtest);
        this_thread::sleep_for(chrono::seconds(1));
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

// FIXME testing functions
string rtest(char buffer[], int n){
    string res = "";
    for (int i = n - 2; i >= 0; i--)
        res += buffer[i];
    return res + '\n';
}
void cmdtest(){
    syslog(LOG_ALERT, "Test of cmd handler!");
}