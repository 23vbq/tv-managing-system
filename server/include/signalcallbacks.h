#ifndef SIGNAL_CALLBACKS_H
#define SIGNAL_CALLBACKS_H
namespace SignalCallbacks{
    static bool* mainloopPtr;

    void SetupCallbacks(bool *mainloop);
    void termination_callback(int);
}
#endif