#include "signalcallbacks.h"

void SignalCallbacks::termination_callback(int sig){
    using namespace std;
    syslog(LOG_INFO, "Closing daemon");
    *s_termination = true;
}
void SignalCallbacks::SetupCallbacks(bool * termination){
    s_termination = termination;
    oldHandler = signal(SIGTERM, termination_callback);
}
void SignalCallbacks::RevertCallbacks(){
    signal(SIGTERM, oldHandler);
}