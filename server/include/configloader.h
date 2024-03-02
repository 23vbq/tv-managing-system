#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include <string>
#include <unordered_map>
#include <fstream>
#include <string>
#include <syslog.h>

using namespace std;

class ConfigLoader{
    private:
        string m_path;
        unordered_map<string, string> m_properties;

        ifstream* OpenFile();
    public:
        // ConfigLoader();
        ConfigLoader(string);
        string GetPath();
        void Load(); // musi rzucać exepszyn
        // void Load(string); // może się tego pozbyć i filenae jako const?
        template <typename T>
        bool GetProperty(string key, T& valueOut);
}
#endif