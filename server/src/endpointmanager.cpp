#include "endpointmanager.h"

// Static variables

const std::string EndpointManager::SETTINGS_EXTENSION = ".ep";

// Constructors

EndpointManager::EndpointManager(string configPath){
    // Initialize local variables
    m_configPath = configPath;
    // Create global Endpoint Settings
    EndpointConnection global;
    global.ip = "GLOBAL";
    global.port = 0;
    global.settings.name = "[GlobalSettings]";
    m_data.push_back(global);
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
    out<<"LocalCfg = "<<to_string(ep->localcfg)<<"\n";
    out<<"Dir = "<<ep->dir<<"\n";
    out<<"Showtime = "<<to_string(ep->showtime)<<"\n";
    out.close();
}

// Public functions

void EndpointManager::Loop(){
    // Save changed settings to file
    size_t n = m_toSave.size();
    vector<size_t> delIters;
    for (size_t i = 0; i < n; i++){
        try{
            SaveSettingsFile(i, delIters);
        } catch (const char* e){
            syslog(LOG_ERR, "[EndpointManager] Loop: %s", e);
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
    syslog(LOG_INFO, "Successfully changed settings for endpoint: %s", &ptr->name[0]);
}