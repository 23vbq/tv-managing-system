#include "configloader.h"

ifstream* ConfigLoader::OpenFile(){
    ifstream* in = new ifstream;
    in->open(m_path);
    if (!in->is_open()){
        delete in;
        throw "Cannot open file: " + m_path;
    }
    return in;
}

ConfigLoader::ConfigLoader(string path){
    m_path = path;
}
string ConfigLoader::GetPath(){
    return m_path;
}
void ConfigLoader::Load(){
    using namespace std;
    syslog(LOG_INFO, "Loading config file: %s", &m_path[0]);
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
        if (line[0] == '#')
            continue;
        // Remove comment part
        size_t sharpPos = line.find('#');
        if (sharpPos != string::npos){
            line = line.substr(0, sharpPos);
        }
        // Handle if equals not found
        size_t eqPos = line.find('=');
        if (eqPos == string::npos){
            syslog(LOG_ERR, "Invalid data at line %i in file %s", n, &m_path[0]);
            continue;
        }
        // Get key and validate if exists
        key = line.substr(0, eqPos);
        if (m_properties.count(key) != 0){
            syslog(LOG_WARNING, "Key %s at line %i already set. Skipping.", &key[0], n);
            continue;
        }
        // Set kvp in map
        value = line.substr(eqPos + 1);
        m_properties[key] = value;
        syslog(LOG_DEBUG, "%s => %s", &key[0], &value[0]);
    }
    // Clean-up
    in->close();
    delete in;
}