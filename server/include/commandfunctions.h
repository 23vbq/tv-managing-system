#ifndef COMMAND_FUNCTIONS_H
#define COMMAND_FUNCTIONS_H

#define CF_ARGS vector<string> x, string& output

#include "serversocket.h"
#include "commandhandler.h"
#include "authmanager.h"

#include <string>
#include <vector>
#include <syslog.h>

using namespace std;

namespace CommandFunctions{
    void disconnect(CF_ARGS); // Disconnect client [no args]
    void getEndpointSettingsByName(CF_ARGS); // Returns endpoint settings [endpoint name]
    void getEndpointList(CF_ARGS); // Returns list of endpoints [no args]
    void setEndpointSettings(CF_ARGS); // Sets settings for endpoint [endpoint name, serialized settings]
    void ping(CF_ARGS); // Pings endpoint [endpoint name]
    void authKey(CF_ARGS); // Authenticate user with key [key]
    void hello(CF_ARGS); // Returns hello message [no args]
}

#endif