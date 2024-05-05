#ifndef ENDPOINT_CONNECTION_H
#define ENDPOINT_CONNECTION_H

#include "endpointsettings.h"
#include "clientsocket.h"

#include <string>

using namespace std;

struct EndpointConnection
{
    string ip;
    unsigned short port;
    EndpointSettings settings;
    ClientSocket* socket = nullptr;
};

#endif