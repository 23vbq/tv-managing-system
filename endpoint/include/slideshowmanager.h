#ifndef SLIDESHOW_MANAGER_H
#define SLIDESHOW_MANAGER_H

#include <string>
#include <vector>
#include <set>
#include <filesystem>
#include <syslog.h>

namespace fs = std::filesystem;

class SlideshowManager{
private:
    static const std::string s_openProg;
    static const std::set<std::string> s_extensions;

    std::vector<std::string> m_paths;

    void ExtensionToLower(std::string&);
    
public:
    bool GetFilesInPath(const std::string&);
    // void OpenImage(const int&);
    void OpenAllImages();
    void ReloadImages(const std::string &);
};

#endif