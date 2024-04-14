#ifndef SERVER_SETTINGS_H
#define SERVER_SETTINGS_H

#include <string>

using namespace std;

struct ServerSettings
{
    string listeningIp;
    unsigned short listeningPort;
    string passwordHash; // FIXME is this good? maybe store in some keyrings
    int maxConnections;
};

#endif