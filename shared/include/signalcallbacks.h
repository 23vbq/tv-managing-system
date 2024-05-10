#ifndef SIGNAL_CALLBACKS_H
#define SIGNAL_CALLBACKS_H

#include <signal.h>
#include <syslog.h>

namespace SignalCallbacks{
    static bool* s_termination; // Pointer to termination bool

    static sighandler_t oldHandler; // Handler to default SIGTERM callback

    /**
     * Sets s_termination bool to true
    */
    void termination_callback(int);
    
    /**
     * Initializes new callbacks
     * @param termination pointer to termination bool
    */
    void SetupCallbacks(bool*);
    /**
     * Reverts callbacks to old
    */
    void RevertCallbacks();
}
#endif