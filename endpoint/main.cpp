#ifdef DEBUG

#define _LOGMASK LOG_UPTO (LOG_DEBUG)
#define _CONFIG_PATH "/home/_vbq/cpp/tv-managing-system/endpoint/test/"

#else

#define _LOGMASK LOG_UPTO (LOG_INFO)
#define _CONFIG_PATH "/etc/tmse/"

#endif

#include <syslog.h>

#include "signalcallbacks.h"
#include "configloader.h"
#include "endpointserversettings.h"


const int LOGMASK = _LOGMASK;
const char* PROCNAME = "tmse";

/*
 * Config
*/
const char* CONFIG_PATH = _CONFIG_PATH;
const char* CONFIG_SETTINGS_FILE = "serversocket.cfg"; // Inside CONFIG_PATH

/*
 * Global loop
*/
static bool s_termination = false;

/*
 * Structs
*/
EndpointServerSettings m_endpointserversettings;

using namespace std;

void LoadServerConfig();
void CleanUp();

int main(int argc, char* argv[]){
    openlog(PROCNAME, LOG_CONS | LOG_PID, LOG_USER);
    setlogmask(LOGMASK);
    syslog(LOG_INFO, "Starting endpoint");
    // Create signal handles
    SignalCallbacks::SetupCallbacks(&s_termination);
    // Load config
    LoadServerConfig();
    CleanUp();
    return 0;
}
void LoadServerConfig(){
    ConfigLoader cfgl = ConfigLoader((string) CONFIG_PATH + CONFIG_SETTINGS_FILE);
    cfgl.Load();
    cfgl.GetProperty<unsigned short>("ListeningPort", m_endpointserversettings.listeningPort);
}
void CleanUp(){
    SignalCallbacks::RevertCallbacks();
}