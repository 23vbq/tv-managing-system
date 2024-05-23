#ifndef SLIDESHOW_MANAGER_H
#define SLIDESHOW_MANAGER_H

#include <string>
#include <vector>
#include <filesystem>
#include <syslog.h>

namespace fs = std::filesystem;

class SlideshowManager{
private:
    static const std::string s_openProg;

    std::vector<std::string> m_paths;
public:
    void GetFilesInPath(const std::string&);
    void OpenImage(const int&);
};

#endif