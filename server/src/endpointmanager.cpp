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