#include <syslog.h>

#include "signalcallbacks.h"
#include "configloader.h"

// For testing
#include <chrono>
#include <thread>
#include <iostream>

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
    int l = 0;
    bool b1 = cfgl.GetProperty<int>("liczba", l);
    bool b2 = cfgl.GetProperty<int>("testing", l);
    while (mainloop)
    {
        syslog(LOG_NOTICE, "Test message");
        this_thread::sleep_for(chrono::seconds(10));
    }
    return 0;
}