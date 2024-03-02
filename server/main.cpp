#include <syslog.h>

#include "signalcallbacks.h"
#include "configloader.h"

// For testing
#include <chrono>
#include <thread>

static bool mainloop = true;

const int LOGMASK = LOG_UPTO (LOG_DEBUG);
const char* DAEMONNAME = "tmsd";

using namespace std;

int main(int argc, char* argv[]){
    // Open syslog
    openlog(DAEMONNAME, LOG_CONS | LOG_PID, LOG_USER);
    setlogmask(LOGMASK);
    syslog(LOG_INFO, "Starting server daemon");
    // Create signal handles
    SignalCallbacks::SetupCallbacks(&mainloop);
    // FIXME Test
    ConfigLoader cfgl = ConfigLoader("./test/test.cfg");
    cfgl.Load();
    /*while (mainloop)
    {
        syslog(LOG_NOTICE, "Test message");
        this_thread::sleep_for(chrono::milliseconds(1000));
    }*/
    return 0;
}