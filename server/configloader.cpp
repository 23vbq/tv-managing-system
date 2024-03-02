#include "configloader.h"

std::ifstream* ConfigLoader::OpenFile(){
    std::ifstream* in = new ifstream;
    in->open(m_path);
    if (!in->is_open())
        throw "Cannot open file: " + m_path;
    return in;
}

ConfigLoader::ConfigLoader(string path){
    m_path = path;
}
string ConfigLoader::GetPath(){
    return m_path;
}
void ConfigLoader::Load(){
    syslog(LOG_INFO, "Loading config file: " + m_path);
    // Clean map
    m_properties.clear();
    // Load file
    ifstream* in = OpenFile();
    string line, key, value;
    int n = 0;
    while (getline(*in, line)){
        n++;
        if (line.length() < 1)
            continue;
        if (line[0] = '#')
            continue;
        // Remove comment part
        size_t sharpPos = line.fine('#');
        if (sharpPos != string::npos){
            line = line.susbtr(0, sharpPos);
        }
        // Handle if equals not found
        size_t eqPos = line.find('=');
        if (eqPos == string::npos){
            syslog(LOG_ERR, "Invalid data at line " + to_string(n) + " in file " + m_path);
            continue;
        }
        // Get key and validate if exists
        key = line.substr(0, eqPos);
        if (m_properties.count(key) != 0){
            syslog(LOG_WARNING, "Key " + key + " at line " + to_string(n) + " already set. Skipping.");
            continue;
        }
        // Set kvp in map
        value = line.substr(eqPos + 1);
        m_properties[key] = value;
        syslog(LOG_DEBUG, key + " => " + value);
    }
    // Clean-up
    in->close();
    delete in;
}