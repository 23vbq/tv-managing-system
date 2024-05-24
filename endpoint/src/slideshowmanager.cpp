#include "slideshowmanager.h"

const std::string SlideshowManager::s_openProg = "feh -Y -Z";
const std::set<std::string> SlideshowManager::s_extensions = {
    ".JPG",
    ".PNG"
};

// Public functions

bool SlideshowManager::GetFilesInPath(const std::string &path){
    int n = 0;
    if (!fs::exists(path))
        return false;
    m_paths.clear();
    for (const auto &x : fs::directory_iterator(path)){
        if (x.is_directory())
            continue;
        // if (x.path().extension().)
        m_paths.push_back(x.path());
        n++;
    }
    syslog(LOG_INFO, "[SlideshowManager] Loaded %i images", n);
    return true;
}
void SlideshowManager::OpenAllImages(){
    for (const auto &x : m_paths){
        system(&(s_openProg + ' ' + x)[0]);
    }
}