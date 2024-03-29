#include "endpointmanager.h"

// Static variables

const string EndpointManager::SETTINGS_EXTENSION = ".ep";

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

// Public functions

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
void EndpointManager::LoadSettingsData(string path){
    namespace fs = filesystem;
    if (!fs::exists(path))
        throw "Endpoint settings path doesn't exists";
    for (EndpointConnection& x : m_data){
        syslog(LOG_INFO, "Loading endpoint settings: %s", &x.settings.name[0]);
        string file = path + x.settings.name + SETTINGS_EXTENSION;
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
    syslog(LOG_INFO, "Successfully changed settings for endpoint: %s", &ptr->name[0]);
}