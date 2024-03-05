#ifndef ENDPOINT_CONNECTION_H
#define ENDPOINT_CONNECTION_H

#include <string>

using namespace std;

struct EndpointConnection
{
    string name;
    string ip;
    unsigned short port;
};

#endif