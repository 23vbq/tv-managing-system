#ifndef ENDPOINT_MANAGER_H
#define ENDPOINT_MANAGER_H

#define EP_GLOBAL_IP "GLOBAL"
#define EP_GLOBAL_PORT 0

#include "endpointconnection.h"
#include "configloader.h"
#include "serializer.h"
#include "serversocket.h"
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
    vector<EndpointConnection*> m_toUpdate; // List of pointers to endpoint connection that update need to be sent

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

    /**
     * Connects EndpointConnection instance socket to
     * IP and port stored in EndpointConnection
    */
    bool ConnectSocket(EndpointConnection*);
    /**
     * Returns pointer to socket of endpoint
     * @param name name of endpoint
     * @return NULL if endpoint not found
    */
    ClientSocket* GetSocket(const string&);
    /**
     * Returns pointer to endpoint
     * @param name name of endpoint
     * @return NULL if endpoint not found
    */
    EndpointConnection* GetEndpoint(const string&);
public:
    /**
     * Constructor of endpoint manager
     * @param configPath path to directory of config files
    */
    EndpointManager(string configPath);
    ~EndpointManager();

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
     * @return NULL if endpoint not found
    */
    EndpointSettings* GetSettings(string&);
    /**
     * Returns settings of endpoint
     * @param ip address of endpoint
     * @param port listening port of endpoint
     * @deprecated Use `GetSettings(string& name)`
     * @return NULL if endpoint not found
    */
    __attribute__ ((deprecated)) EndpointSettings* GetSettings(string&, unsigned short&);

    /**
     * Changes settings of endpoint. Marks endpoint to save and update.
     * @param ptr pointer to endpoint to update
     * @param settings new settings
    */
    void SetSettings(EndpointSettings*, EndpointSettings&);

    /**
     * Initializes sockets to all endpoints.
     * Marks all initialized and connected sockets to update.
    */
    void InitializeEndpointSockets();
    /**
     * Sends provided message to all endpoints
     * @param message data to send
    */
    void SendToAll(const string&);
    bool SendToOne(EndpointConnection*, const string&, string*);
    /**
     * Sends provided message to one endpoint specified by name
     * @param name endpoint name to send to
     * @param message data to send
     * @param result pointer to result string (pass NULL if not require result)
     * @return True if message was sent, False if error on sending or can't find socket with name
    */
    bool SendToOne(const string&, const string&, string*);
    /**
     * Sends update to endpoints that are marked to update.
    */
    void SendUpdate();
};

#endif