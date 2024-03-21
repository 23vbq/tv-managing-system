#include "endpointmanager.h"

// Static variables

const string EndpointManager::SETTINGS_EXTENSION = ".ep";

// Private functions

bool EndpointManager::LoadSettingsFile(const string& filePath, EndpointSettings& endpoint){
    ConfigLoader cfg = ConfigLoader(filePath);
    cfg.Load();
    return cfg.GetProperty<bool>("LocalCfg", endpoint.localcfg) &&
           cfg.GetProperty<string>("Dir", endpoint.dir) &&
           cfg.GetProperty<unsigned int>("Showtime", endpoint.showtime);
}

// Public functions

void EndpointManager::LoadConnectionData(vector<Config>* endpoints){
    EndpointConnection buffer;
    for (Config& x : *endpoints){
        if (x.GetProperty<string>("Name", buffer.settings.name) &&
            x.GetProperty<string>("Ip", buffer.ip) &&
            x.GetProperty<unsigned short>("Port", buffer.port)){
            m_data.push_back(buffer);
            syslog(LOG_DEBUG, "Loaded endpoint connection: %s", &buffer.settings.name[0]);
        } else{
            syslog(LOG_DEBUG, "Failure on loading EndpointConnection");
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
        if (LoadSettingsFile(file, x.settings)){
            syslog(LOG_DEBUG, "Loaded endpoint settings: %s", &x.settings.name[0]);
        } else{
            syslog(LOG_WARNING, "Cannot load endpoint settings: %s", &x.settings.name[0]);
        }
    }
    // for (const filesystem::directory_entry& file : filesystem::directory_iterator(path)){
    //     LoadSettingsFile(file.path());
    // }
    // FIXME test
    /*EndpointSettings est(m_data[0].settings.name, true, "/mnt/images", 15);
    m_data[0].settings = est;
    est = EndpointSettings(m_data[1].settings.name, false, "/mnt/server/obrazki/telewizory/", 32);
    m_data[1].settings = est;*/
}

vector<string>* EndpointManager::GetNames(){
    vector<string>* result = new vector<string>;
    for (const EndpointConnection& x : m_data){
        result->push_back(x.settings.name);
    }
    return result;
}
EndpointSettings* EndpointManager::GetSettings(string& name){
    for (const EndpointConnection& x : m_data){
        if (x.settings.name == name)
            return new EndpointSettings(x.settings);
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