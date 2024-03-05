#ifndef CONFIG_H
#define CONFIG_H

#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <syslog.h>

using namespace std;

class Config
{
    protected:
        unordered_map<string, string> m_properties;
    public:
        template <typename T>
        bool GetProperty(string key, T& valueOut){
            if (m_properties.size() == 0){
                syslog(LOG_WARNING, "Config not loaded or is empty.");
                return false;
            }
            if (!m_properties.count(key)){
                syslog(LOG_WARNING, "Property [%s] not found!", &key[0]);
                return false;
            }
            // Handle if type is string
            if (typeid(T) == typeid(string)){
                valueOut = m_properties[key];
                return true;
            }
            stringstream ss;
            ss<<m_properties[key];
            ss>>valueOut;
            return true;
        }
        vector<Config>* GetList(string);
        string ToString();
};

#endif