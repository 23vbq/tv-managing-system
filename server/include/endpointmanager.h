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
    static const string SETTINGS_EXTENSION; // Endpoint config file extension
private:
    vector<EndpointConnection> m_data; // Stores loaded endpoints
    vector<EndpointSettings*> m_toSave; // List of pointers to endpoint settings that need to be saved

    string m_configPath; // Path to directory of config files

    /**
     * Opens endpoint file and load properties
     * @param filePath path to file to open
     * @param endpoint reference to output of loaded properties
     * @return Failcode created with `Config.GetProperty()`
    */
    int LoadSettingsFile(const string&, EndpointSettings&);
    /**
     * Saves endpoint settings to config file.
     * File is saved in `m_configPath` location with name of endpoint name
     * @param iterator iterator of `m_toSave` vector
     * @param deleteList list of successfully saved settings
     * @throw `const char*` - message [ep == NULL, !out.is_open()]
    */
    void SaveSettingsFile(size_t&, vector<size_t>&);
public:
    /**
     * Constructor of endpoint manager
     * @param configPath path to directory of config files
    */
    EndpointManager(string configPath);

    /**
     * Starts saving endpoints settings that was updated, and need to be saved.
    */
    void SaveSettings();

    void LoadConnectionData(vector<Config>*);
    void LoadSettingsData();

    vector<string>* GetNames();
    EndpointSettings* GetSettings(string&);
    EndpointSettings* GetSettings(string&, unsigned short&);

    void SetSettings(EndpointSettings*, EndpointSettings&);
};

#endif