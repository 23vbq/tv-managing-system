#ifndef COMMAND_FUNCTIONS_H
#define COMMAND_FUNCTIONS_H

#include <string>
#include <vector>
#include <syslog.h>

using namespace std;

namespace CommandFunctions{
    // TODO Real commands not for testing
    void rtest(vector<string> x, string& output);
    void cmdtest(vector<string> x, string& output);
}

#endif