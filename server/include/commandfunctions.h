#ifndef COMMAND_FUNCTIONS_H
#define COMMAND_FUNCTIONS_H

#include "serversocket.h"
#include "commandhandler.h"

#include <string>
#include <vector>
#include <syslog.h>

using namespace std;

namespace CommandFunctions{
    void disconnect(vector<string> x, string& output); // Disconnect client [no args]
    void getEndpointSettingsByName(vector<string> x, string& output); // Returns endpoint settings [endpoint name]
    void getEndpointList(vector<string> x, string& output); // Returns list of endpoints [no args]
    void setEndpointSettings(vector<string> x, string& output); // Sets settings for endpoint [endpoint name, serialized settings]
    // TODO Real commands not for testing
    void rtest(vector<string> x, string& output);
}

#endif