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
    ConfigLoader cfgl = ConfigLoader("/home/_vbq/cpp/tv-managing-system/server/test/test.cfg");
    cfgl.Load();
    int x = 0, x2 = 0;
    bool b1 = cfgl.GetProperty<int>("liczba", x);
    bool b2 = cfgl.GetProperty<int>("testing", x);
    vector<Config>* l1 = cfgl.GetList("lista");
    for (int i = 0; i < l1->size(); i++)
        syslog(LOG_CRIT, "%s", &(l1->at(i).ToString())[0]);
    bool b3 = l1->at(0).GetProperty<int>("fajne", x2);
    while (mainloop)
    {
        syslog(LOG_NOTICE, "Test message");
        this_thread::sleep_for(chrono::seconds(10));
    }
    delete l1;
    syslog(LOG_INFO, "Daemon exited successfully");
    return 0;
}