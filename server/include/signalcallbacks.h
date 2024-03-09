#ifndef SIGNAL_CALLBACKS_H
#define SIGNAL_CALLBACKS_H

#include <signal.h>
#include <syslog.h>

namespace SignalCallbacks{
    static bool* s_termination;

    static sighandler_t oldHandler;

    void termination_callback(int);
    
    void SetupCallbacks(bool*);
    void RevertCallbacks();
}
#endif