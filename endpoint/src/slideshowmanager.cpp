#include "slideshowmanager.h"

const std::string SlideshowManager::s_openProg = "feh -Y -Z";

// Public functions

void SlideshowManager::GetFilesInPath(const std::string &path){
    int n = 0;
    m_paths.clear();
    for (const auto &x : fs::directory_iterator(path)){
        m_paths.push_back(x.path());
        n++;
    }
    syslog(LOG_INFO, "[SlideshowManager] Loaded %i images", n);
}
void SlideshowManager::OpenAllImages(){
    for (const auto &x : m_paths){
        system(&(s_openProg + ' ' + x)[0]);
    }
}