#ifndef ENDPOINT_MANAGER_H
#define ENDPOINT_MANAGER_H

#include "endpointconnection.h"
#include "configloader.h"

#include <vector>
#include <string>
#include <filesystem>

using namespace std;

class EndpointManager{
public:
    static const string SETTINGS_EXTENSION;
private:
    vector<EndpointConnection> m_data;

    int LoadSettingsFile(const string&, EndpointSettings&);
public:
    void LoadConnectionData(vector<Config>*);
    void LoadSettingsData(string);

    vector<string>* GetNames();
    EndpointSettings* GetSettings(string&);
    EndpointSettings* GetSettings(string&, unsigned short&);

    void SetSettings(EndpointSettings*, EndpointSettings&);
};

#endif