#include "endpointmanager.h"

// Public functions

void EndpointManager::LoadConnectionData(vector<Config>* endpoints){
    EndpointConnection buffer;
    for (Config& x : *endpoints){
        if (x.GetProperty<string>("Name", buffer.settings.name) &&
            x.GetProperty<string>("Ip", buffer.ip) &&
            x.GetProperty<unsigned short>("Port", buffer.port)){
            m_data.push_back(buffer);
            syslog(LOG_DEBUG, "Loaded endpoint: %s", &buffer.settings.name[0]);
        } else{
            syslog(LOG_DEBUG, "Failure on loading EndpointConnection");
        }
    }
}
void EndpointManager::LoadSettingsData(string path){
    // FIXME test
    EndpointSettings est(m_data[0].settings.name, true, "/mnt/images", 15);
    m_data[0].settings = est;
    est = EndpointSettings(m_data[1].settings.name, false, "/mnt/server/obrazki/telewizory/", 32);
    m_data[1].settings = est;
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