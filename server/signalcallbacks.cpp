#include "signalcallbacks.h"

void SignalCallbacks::termination_callback(int sig){
    using namespace std;
    syslog(LOG_INFO, "Closing daemon");
    *mainloopPtr = false;
}
void SignalCallbacks::SetupCallbacks(bool * mainloop){
    mainloopPtr = mainloop;
    signal(SIGTERM, termination_callback);
}