#ifndef SIGNAL_CALLBACKS_H
#define SIGNAL_CALLBACKS_H

#include <signal.h>
#include <syslog.h>

namespace SignalCallbacks{
    static bool* s_mainloopPtr;

    void SetupCallbacks(bool *mainloop);
    void termination_callback(int);
}
#endif