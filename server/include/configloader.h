#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <syslog.h>

using namespace std;

class ConfigLoader{
    private:
        string m_path;
        unordered_map<string, string> m_properties;

        ifstream* OpenFile();
        bool kvpCheck(string&, int&, size_t&);
        void ClearWhitespaces(string&);
        // void LoadList(string&, string&, bool&, int&);
    public:
        // ConfigLoader();
        ConfigLoader(string);

        string GetPath();
        bool isEmpty();

        void Load(); // musi rzucać exepszyn
        // void Load(string); // może się tego pozbyć i filenae jako const?
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
};
#endif