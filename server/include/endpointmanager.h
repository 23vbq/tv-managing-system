#ifndef ENDPOINT_MANAGER_H
#define ENDPOINT_MANAGER_H

#include "endpointconnection.h"
#include "configloader.h"
// #include "actionqueue.h"

#include <vector>
#include <string>
#include <filesystem>

using namespace std;

class EndpointManager{
public:
    static const string SETTINGS_EXTENSION;
private:
    vector<EndpointConnection> m_data;
    vector<EndpointSettings*> m_toSave;

    string m_configPath;

    int LoadSettingsFile(const string&, EndpointSettings&);
    void SaveSettingsFile(size_t&, vector<size_t>&);
public:
    EndpointManager(string configPath);

    void SaveSettings();

    void LoadConnectionData(vector<Config>*);
    void LoadSettingsData();

    vector<string>* GetNames();
    EndpointSettings* GetSettings(string&);
    EndpointSettings* GetSettings(string&, unsigned short&);

    void SetSettings(EndpointSettings*, EndpointSettings&);
};

#endif