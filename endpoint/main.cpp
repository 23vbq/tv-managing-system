#ifdef DEBUG

#define _LOGMASK LOG_UPTO (LOG_DEBUG)
#define _CONFIG_PATH "/home/_vbq/cpp/tv-managing-system/server/test/"

#else

#define _LOGMASK LOG_UPTO (LOG_INFO)
#define _CONFIG_PATH "/etc/tmsd/"

#endif

#include <syslog.h>

#include "signalcallbacks.h"


const int LOGMASK = _LOGMASK;
const char* DAEMONNAME = "tmsd";

/*
 * Config
*/
const char* CONFIG_PATH = _CONFIG_PATH;
const char* CONFIG_SETTINGS_FILE = "serversocket.cfg"; // Inside CONFIG_PATH

/*
 * Global loop
*/
static bool s_termination = false;

using namespace std;

int main(int argc, char* argv[]){
    openlog(DAEMONNAME, LOG_CONS | LOG_PID, LOG_USER);
    setlogmask(LOGMASK);
    syslog(LOG_INFO, "Starting endpoint");
    // Create signal handles
    SignalCallbacks::SetupCallbacks(&s_termination);
    return 0;
}