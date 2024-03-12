#include "commandfunctions.h"

namespace CommandFunctions{
    void rtest(vector<string> x, string& output){
        size_t n = x[0].length();
        output = "";
        for (int i = n - 2; i >= 0; i--)
            output += x[0][i];
        output += '\n';
    }
    void cmdtest(vector<string> x, string& output){
        syslog(LOG_ALERT, "Function said: %s %i", &(x[0])[0], stoi(x[1]));
        output = "fajne";
    }
}