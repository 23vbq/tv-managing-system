#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include "config.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
#include <syslog.h>

using namespace std;

class ConfigLoader : public Config{
    private:
        string m_path;

        ifstream* OpenFile();
        void ClearWhitespaces(string&);
        // void LoadList(string&, string&, bool&, int&);
    public:
        // ConfigLoader();
        ConfigLoader(string);

        string GetPath();
        bool isEmpty();

        void Load(); // musi rzucać exepszyn
        // void Load(string); // może się tego pozbyć i filenae jako const?
};
#endif