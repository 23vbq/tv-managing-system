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
void ConfigLoader::ClearWhitespaces(string& line){
    // FIXME do not remove normal spaces if inside of value
    int n = line.length();
    for (int i = 0; i < n; i++){
        if (line[i] == ' ' || line[i] == '\t'){
            line.erase(i--, 1);
            n--;
        }
    }
}
// void ConfigLoader::LoadList(string& key, string& line, bool& isList, int& n){
//     m_properties[key] += line
// }

ConfigLoader::ConfigLoader(string path){
    m_path = path;
}

string ConfigLoader::GetPath(){
    return m_path;
}

void ConfigLoader::Load(){
    if (m_path == ""){
        syslog(LOG_WARNING, "Trying to load config without path!");
        return;
    }
    using namespace std;
    syslog(LOG_INFO, "Loading config file: %s", &m_path[0]);
    // Clean map
    m_properties.clear();
    // Load file
    ifstream* in = OpenFile(); // FIXME czy tu catchować
    string line, key, value;
    int n = 0;
    bool isList = false;
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
        // Remove whitespaces
        ClearWhitespaces(line);
        size_t eqPos;
        // Handle if is list
        if (isList){
            // LoadList(key, line, isList, n);
            // End list check
            if (line.find(']') != string::npos){
                isList = false;
                continue;
            }
            // Handle if equals not found
            if ((eqPos = line.find('=')) == string::npos && line.find('{') == string::npos && line.find('}') == string::npos){
                syslog(LOG_ERR, "Invalid data at line %i in file %s", n, &m_path[0]);
                continue;
            }
            // Add to value
            m_properties[key] += line + '\n';
            syslog(LOG_DEBUG, "List line: %s", &line[0]);
            continue;
        }
        // Handle if equals not found
        if ((eqPos = line.find('=')) == string::npos){
            syslog(LOG_ERR, "Invalid data at line %i in file %s", n, &m_path[0]);
            continue;
        }
        // Get key and validate if exists
        key = line.substr(0, eqPos);
        if (m_properties.count(key) != 0){
            syslog(LOG_WARNING, "Key %s at line %i already set. Skipping.", &key[0], n);
            continue;
        }
        // Get value
        value = line.substr(eqPos + 1);
        // Open list check
        if (value.find('[') != string::npos){
            isList = true;
            m_properties[key] = "";
            continue;
        }
        // Set value
        m_properties[key] = value;
        syslog(LOG_DEBUG, "%s => %s", &key[0], &value[0]);
    }
    // Clean-up
    in->close();
    delete in;
}