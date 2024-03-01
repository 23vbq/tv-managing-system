#include <syslog.h>
#include "signalcallbacks.h"
// For testing
#include <chrono>
#include <thread>

static bool mainloop = true;
const char* DAEMONNAME = "tmsd";

using namespace std;

int main(int argc, char* argv[]){
    // Open syslog
    openlog(DAEMONNAME, LOG_CONS | LOG_PID, LOG_USER);
    syslog(LOG_INFO, "Starting server daemon");
    // Create signal handles
    SignalCallbacks::SetupCallbacks(&mainloop);
    while (mainloop)
    {
        syslog(LOG_NOTICE, "Test message");
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
    return 0;
}