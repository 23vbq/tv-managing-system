#ifndef COMMAND_FUNCTIONS_H
#define COMMAND_FUNCTIONS_H

#include "serversocket.h"
#include "commandhandler.h"

#include <string>
#include <vector>
#include <syslog.h>

using namespace std;

namespace CommandFunctions{
    void disconnect(vector<string> x, string& output);
    // TODO Real commands not for testing
    void rtest(vector<string> x, string& output);
    void cmdtest(vector<string> x, string& output);
}

#endif