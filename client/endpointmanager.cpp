#include "endpointmanager.h"

extern ClientSocketQt* m_ClientSocket;

// Public functions

bool EndpointManager::GetEndpointSettings(std::string& name, EndpointSettings& outEndpoint){
    if (name == m_settings.name){
        outEndpoint = m_settings;
        return true;
    }
    m_ClientSocket->Send("GETEPSET " + name);
    std::string result;
    QMessageBox msg;
    if (!m_ClientSocket->Read(result)){
        msg.critical(nullptr, "Connection", "Cannot load endpoint settings");
        return false;
    }
    Serializer sr(result);
    try{
        m_settings.name = sr.DeserializeNext();
        m_settings.localcfg = sr.DeserializeNext<bool>();
        m_settings.dir = sr.DeserializeNext();
        m_settings.showtime = sr.DeserializeNext<unsigned int>();
    } catch (const char * e){
        msg.critical(nullptr, "Serializer", e);
        return false;
    }
    outEndpoint = m_settings;
    return true;
}
