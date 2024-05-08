#ifndef COMMAND_FUNCTIONS_H
#define COMMAND_FUNCTIONS_H

#define CF_ARGS vector<string> x, string& output

#include "serversocket.h"
#include "commandhandler.h"
#include "settingsmanager.h"

#include <string>
#include <vector>
#include <syslog.h>

using namespace std;

namespace CommandFunctions{
    void disconnect(CF_ARGS); // Disconnect client [no args]
    void setEndpointSettings(CF_ARGS); // Sets settings for endpoint [endpoint name, serialized settings]
    // void authKey(CF_ARGS); // Authenticate user with key
    void hello(CF_ARGS); // Returns hello message
    void ping(CF_ARGS); // Returns timestamp from server
}

#endif