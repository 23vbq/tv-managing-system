#ifndef ENDPOINT_MANAGER_H
#define ENDPOINT_MANAGER_H

#include "endpointconnection.h"
#include "config.h"

#include <vector>

using namespace std;

class EndpointManager{
private:
    vector<EndpointConnection> m_data;
public:
    void LoadConnectionData(vector<Config>*);
    void LoadSettingsData(string);

    vector<string>* GetNames();
    EndpointSettings* GetSettings(string&);
    EndpointSettings GetSettings(string, unsigned short);
};

#endif