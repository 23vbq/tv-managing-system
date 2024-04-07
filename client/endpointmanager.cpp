#include "endpointmanager.h"

extern ClientSocketQt* m_ClientSocket;

// Public functions

bool EndpointManager::GetEndpointSettings(std::string& name, EndpointSettings& outEndpoint){
    /*if (name == m_settings.name){
        outEndpoint = m_settings;
        return true;
    }*/
    if (!m_ClientSocket->IsConnected())
        return false;
    m_ClientSocket->Send("GETEPSET " + name);
    std::string result;
    if (!m_ClientSocket->Read(result)){
        return false;
    }
    Serializer sr(result);
    try{
        m_settings.name = sr.DeserializeNext();
        m_settings.localcfg = sr.DeserializeNext<bool>();
        m_settings.dir = sr.DeserializeNext();
        m_settings.showtime = sr.DeserializeNext<unsigned int>();
    } catch (const char * e){
        QMessageBox msg;
        msg.critical(nullptr, "Serializer", e);
        return false;
    }
    outEndpoint = m_settings;
    return true;
}
bool EndpointManager::SaveEndpointSettings(){
    if (!m_ClientSocket->IsConnected())
        return false;
    Serializer sr;
    sr.AddValue(m_settings.name);
    sr.AddValue<bool>(m_settings.localcfg);
    sr.AddValue(m_settings.dir);
    sr.AddValue<unsigned int>(m_settings.showtime);
    m_ClientSocket->Send("SETEPSET " + m_settings.name + ' ' + sr.Serialize());
    std::string result;
    if (!m_ClientSocket->Read(result) || result != "OK\r\n")
        return false;
    return true;
}
void EndpointManager::SetLocalCfg(const bool& value){
    m_settings.localcfg = value;
}
void EndpointManager::SetDir(const std::string& value){
    m_settings.dir = value;
}
void EndpointManager::SetShowtime(const unsigned int& value){
    m_settings.showtime = value;
}
