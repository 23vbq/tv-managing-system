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
    // TODO Real commands not for testing
    void rtest(vector<string> x, string& output);
    void cmdtest(vector<string> x, string& output);
}

#endif