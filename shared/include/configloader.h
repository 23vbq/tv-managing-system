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

    /**
     * Opens file from m_path
    */
    ifstream* OpenFile();
    /**
     * Removes whitespaces from string
    */
    void ClearWhitespaces(string&);
    // void LoadList(string&, string&, bool&, int&);
public:
    // ConfigLoader();
    /**
     * Constructor for ConfigLoader
     * @param path file path to load
    */
    ConfigLoader(string);

    /**
     * Returns provided path
    */
    string GetPath();

    /**
     * Loads file from path provided in constructor
    */
    void Load(); // FIXME musi rzucać exepszyn
};
#endif