#include "config.h"

// Template specialization

/**
 * Gets string property from key-value pair based config
 * @param key key to search in config
 * @param valueOut variable to store string value of property
 * @return TRUE if property found, FALSE if property not found or config is empty
 */
template <>
bool Config::GetProperty<string>(string key, string& valueOut){
    if (m_properties.size() == 0){
        syslog(LOG_WARNING, "Config not loaded or is empty.");
        return false;
    }
    if (!m_properties.count(key)){
        syslog(LOG_WARNING, "Property [%s] not found!", &key[0]);
        return false;
    }
    valueOut = m_properties[key];
    return true;
}

// Public functions

vector<Config>* Config::GetList(string listKey){
    if (m_properties.size() == 0){
        syslog(LOG_WARNING, "Config not loaded or is empty.");
        return NULL;
    }
    if (!m_properties.count(listKey)){
        syslog(LOG_WARNING, "Property [%s] not found!", &listKey[0]);
        return NULL;
    }
    stringstream ss(m_properties[listKey]);
    syslog(LOG_INFO, "Loading config list: %s", &listKey[0]);
    vector<Config>* result = new vector<Config>();
    Config buffer;
    string line, key, value;
    while (getline(ss, line))
    {
        if (line.find('{') != string::npos){
            buffer.m_properties.clear();
            continue;
        }
        if (line.find('}') != string::npos){
            result->push_back(buffer);
            continue;
        }
        size_t eqPos = line.find('=');
        key = line.substr(0, eqPos);
        value = line.substr(eqPos + 1);
        buffer.m_properties[key] = value;
        syslog(LOG_DEBUG, "%s => %s", &key[0], &value[0]);
    }
    return result;
}
string Config::ToString(){
    string result = "{ ";
    for(pair<string, string> x : m_properties){
        result += x.first + "=" + x.second + " ";
    }
    result += "}";
    return result;
}
bool Config::isEmpty(){
    return m_properties.size() == 0;
}