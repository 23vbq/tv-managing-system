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
    bool ConnectSocket(EndpointConnection*);
    ClientSocket* GetSocket(const string&);
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

    /**
     * Loads Endpoints from config list
     * @param endpoints list of configs
    */
    void LoadConnectionData(vector<Config>*);
    /**
     * Loads settings for all loaded endpoints
     * @throw `const char*` if config path not exists
    */
    void LoadSettingsData();

    /**
     * Returns names of all loaded endpoints
    */
    vector<string>* GetNames();
    /**
     * Returns settings of endpoint
     * @param name name of endpoint
    */
    EndpointSettings* GetSettings(string&);
    /**
     * Returns settings of endpoint
     * @param ip address of endpoint
     * @param port listening port of endpoint
     * @deprecated Use `GetSettings(string& name)`
    */
    __attribute__ ((deprecated)) EndpointSettings* GetSettings(string&, unsigned short&);

    /**
     * Changes settings of endpoint. Marks endpoint to save.
     * @param ptr pointer to endpoint to update
     * @param settings new settings
    */
    void SetSettings(EndpointSettings*, EndpointSettings&);

    void InitializeEndpointSockets();
    void SendToAll(const string&);
    bool SendToOne(const string&, const string&);
};

#endif