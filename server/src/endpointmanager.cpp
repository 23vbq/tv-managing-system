#include "endpointmanager.h"

// extern ActionQueue* m_ActionQueue;

// Static variables

const std::string EndpointManager::SETTINGS_EXTENSION = ".ep";

// Constructors

EndpointManager::EndpointManager(string configPath){
    syslog(LOG_INFO, "[EndpointManager] Initializing");
    // Initialize local variables
    m_configPath = configPath;
    // Create global Endpoint Settings
    EndpointConnection global;
    global.ip = EP_GLOBAL_IP;
    global.port = EP_GLOBAL_PORT;
    global.settings.name = "[Global Settings]";
    m_data.push_back(global);
}
EndpointManager::~EndpointManager(){
    syslog(LOG_INFO, "[EndpointManager] Closing");
    for (const EndpointConnection& x : m_data){
        if (x.socket)
            delete x.socket;
    }
}

// Private functions

int EndpointManager::LoadSettingsFile(const string& filePath, EndpointSettings& endpoint){
    int code = 0;
    ConfigLoader cfg = ConfigLoader(filePath);
    cfg.Load();
    cfg.GetProperty<bool>("LocalCfg", endpoint.localcfg, code);
    cfg.GetProperty<string>("Dir", endpoint.dir, code);
    cfg.GetProperty<unsigned int>("Showtime", endpoint.showtime, code);
    return code;
}
void EndpointManager::SaveSettingsFile(size_t& iterator, vector<size_t>& deleteList){
    EndpointSettings* ep = m_toSave.at(iterator);
    if (ep == NULL)
        throw "Cannot save settings of NULL";
    ofstream out;
    out.open(m_configPath + ep->name + SETTINGS_EXTENSION, ios::out);
    if (!out.is_open())
        throw "Cannot open file";
    out<<"LocalCfg = "<<to_string(ep->localcfg)<<"\n";
    out<<"Dir = "<<ep->dir<<"\n";
    out<<"Showtime = "<<to_string(ep->showtime)<<"\n";
    out.close();
    deleteList.push_back(iterator);
}
bool EndpointManager::ConnectSocket(EndpointConnection *ptr){
    return ptr->socket->Connect(ptr->ip, ptr->port);
}
ClientSocket* EndpointManager::GetSocket(const string &name){
    for (EndpointConnection& x : m_data){
        if (x.settings.name == name)
            return x.socket;
    }
    return NULL;
}
EndpointConnection* EndpointManager::GetEndpoint(const string& name){
    for (EndpointConnection& x : m_data){
        if (x.settings.name == name)
            return &x;
    }
    return NULL;
}

// Public functions

void EndpointManager::SaveSettings(){
    // Save changed settings to file
    size_t n = m_toSave.size();
    vector<size_t> delIters;
    for (size_t i = 0; i < n; i++){
        try{
            SaveSettingsFile(i, delIters);
        } catch (const char* e){
            syslog(LOG_ERR, "[EndpointManager] SaveSettings: %s", e);
        }
    }
    // Clear toSave list
    for (const size_t& i : delIters)
        m_toSave.erase(m_toSave.begin() + i);
}

void EndpointManager::LoadConnectionData(vector<Config>* endpoints){
    EndpointConnection buffer;
    int code;
    for (Config& x : *endpoints){
        buffer = {};
        code = 0;
        x.GetProperty<string>("Name", buffer.settings.name, code);
        x.GetProperty<string>("Ip", buffer.ip, code);
        x.GetProperty<unsigned short>("Port", buffer.port, code);
        x.GetProperty<string>("AuthKey", buffer.authkey, code);
        if (code)
            syslog(LOG_DEBUG, "Failure on loading EndpointConnection. Failcode: %i", code);
        else {
            m_data.push_back(buffer);
            syslog(LOG_DEBUG, "Loaded endpoint connection: %s", &buffer.settings.name[0]);
        }
    }
}
void EndpointManager::LoadSettingsData(){
    namespace fs = filesystem;
    if (!fs::exists(m_configPath))
        throw "Endpoint settings path doesn't exists";
    for (EndpointConnection& x : m_data){
        syslog(LOG_INFO, "Loading endpoint settings: %s", &x.settings.name[0]);
        string file = m_configPath + x.settings.name + SETTINGS_EXTENSION;
        if (!fs::exists(file)){
            syslog(LOG_WARNING, "Settings file doesn't exists for endpoint: %s", &x.settings.name[0]);
            continue;
        }
        int failcode = LoadSettingsFile(file, x.settings);
        if (failcode){
            syslog(LOG_WARNING, "Cannot load endpoint settings for %s with failcode %i", &x.settings.name[0], failcode);
        } else{
            syslog(LOG_DEBUG, "Loaded endpoint settings: %s", &x.settings.name[0]);
        }
    }
}

vector<string>* EndpointManager::GetNames(){
    vector<string>* result = new vector<string>;
    for (const EndpointConnection& x : m_data){
        result->push_back(x.settings.name);
    }
    return result;
}
EndpointSettings* EndpointManager::GetSettings(string& name){
    for (EndpointConnection& x : m_data){
        if (x.settings.name == name)
            return &(x.settings); // new EndpointSettings(x.settings)
    }
    return NULL;
}
EndpointSettings* EndpointManager::GetSettings(string& ip, unsigned short& port){
    for (const EndpointConnection& x : m_data){
        if (x.ip == ip && x.port == port)
            return new EndpointSettings(x.settings);
    }
    return NULL;
}

void EndpointManager::SetSettings(EndpointSettings* ptr, EndpointSettings& settings){ // string& name
    /*EndpointSettings* ep = GetSettings(name);
    if (ep == NULL){
        syslog(LOG_ERR, "Requested SetSettings for invalid endpoint name: %s", &name[0]);
        return false;
    }*/
    ptr->localcfg = settings.localcfg;
    ptr->dir = settings.dir;
    ptr->showtime = settings.showtime;
    m_toSave.push_back(ptr);
    m_toUpdate.push_back(GetEndpoint(ptr->name));
    // m_ActionQueue->Add(this, &EndpointManager::SaveSettings, false);
    syslog(LOG_INFO, "Successfully changed settings for endpoint: %s", &ptr->name[0]);
}
void EndpointManager::InitializeEndpointSockets(){
    EndpointConnection* ptr;
    size_t n = m_data.size();
    syslog(LOG_DEBUG, "Initializing sockets for endpoints");
    for (size_t i = 0; i < n; i++){
        ptr = &(m_data[i]);
        // Ignore if it is GLOBAL endpoint
        if (ptr->ip == EP_GLOBAL_IP)
            continue;

        // Create client socket
        if (ptr->socket == NULL)
            ptr->socket = new ClientSocket();
        // Connect if not connected
        if (!ptr->socket->IsConnected()){
            string r;
            ConnectSocket(ptr);
            // If successfully connected
            if (ptr->socket && ptr->socket->IsConnected()){
                ptr->socket->Read(&r); // Read HELLO
                // Authenticate
                ptr->socket->Send("AUTHK \2kotki145\3");
                ptr->socket->Read(&r); // Read AUTH
                // Add endpoint to update
                m_toUpdate.push_back(ptr);
                syslog(LOG_WARNING, "Successfully connected to endpoint");
            }
        }
    }
}
void EndpointManager::SendToAll(const string &message){
    for (const EndpointConnection& x : m_data){
        if (x.socket == NULL || !x.socket->IsConnected())
            continue;
        x.socket->Send(message);
        x.socket->Read(NULL);
    }
}
bool EndpointManager::SendToOne(const string &name, const string &message, string *result){
    ClientSocket *ptr = GetSocket(name);
    if (!ptr) {
        syslog(LOG_WARNING, "Cannot find socket for endpoint name %s", &name[0]);
        return false;
    }
    bool ret = (ptr->Send(message) && ptr->Read(result));
    if (!ret)
        return ret;
    // Check for authentication
    if (result->find(ServerSocket::SMSG_AUTH_REQ) != string::npos){
        string auth_result;
        EndpointConnection* ep = GetEndpoint(name);
        ptr->Send("AUTHK \2" + ep->authkey + "\3");
        ptr->Read(&auth_result);
        // Authentication failure
        if (auth_result.find("SUCCESS") == string::npos){
            syslog(LOG_ERR, "Cannot authenticate to endpoint %s [%s:%i]", &name[0], &(ep->ip)[0], ep->port);
            return false;
        }
        return SendToOne(name, message, result);
    }
    if (result->find("OK\r\n") != string::npos)
        *result = result->substr(4);
    return ret;
}
void EndpointManager::SendUpdate(){
    size_t n = m_toUpdate.size();
    vector<size_t> delIters;
    EndpointSettings* settings;
    for (size_t i = 0; i < n; i++){
        Serializer sr;
        settings = &(m_toUpdate[i]->settings);
        // Handle local / global settings
        if (!settings->localcfg)
            settings = &(m_data[0].settings);
        // Serialize data
        sr.AddValue(settings->name);
        sr.AddValue<bool>(settings->localcfg);
        sr.AddValue(settings->dir);
        sr.AddValue<unsigned int>(settings->showtime);
        // Send data
        if (m_toUpdate[i]->socket->Send("SETEPSET \2" + sr.Serialize() + "\3")){
            delIters.push_back(i);
            m_toUpdate[i]->socket->Read(NULL);
        }
    }
    // Clear toUpdate list
    for (const size_t& i : delIters)
        m_toUpdate.erase(m_toUpdate.begin() + i);
}