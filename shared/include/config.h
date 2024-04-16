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
    /***
     * Gets any property from key-value pair based config
     * @param key key to search in config
     * @param valueOut variable to store value of property
     * @return TRUE if property found, FALSE if property not found or config is empty
    */
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
        stringstream ss;
        ss<<m_properties[key];
        ss>>valueOut;
        return true;
    }
    /**
     * Gets any property from key-value pair based config.
     * Works on bit failcode (stored in int). Every call shifts bits to left by 1.
     * If GetProperty fails increases failCode (sets last bit to one).
     * @param key key to search in config
     * @param valueOut variable to store value of property
     * @param failCode variable to store bit failcode
     * @return TRUE if property found, FALSE if property not found or config is empty
    */
    template <typename T>
    bool GetProperty(string key, T& valueOut, int& failCode){
        bool res = GetProperty<T>(key, valueOut);
        failCode = failCode<<1;
        if (!res)
            failCode++;
        return res;
    }
    /**
     * Reads list from config.
     * @param listKey key of list to search in config
     * @return Vector of Configs
    */
    vector<Config>* GetList(string);
    /**
     * Combines whole config to string, so can be written to user.
    */
    string ToString();
};

#endif