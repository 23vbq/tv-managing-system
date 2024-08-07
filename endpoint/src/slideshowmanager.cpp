#include "slideshowmanager.h"
#include "windowmanager.h"

extern WindowManager* m_WindowManager;

std::string SlideshowManager::s_openProg = "feh -Y -. -Z -F";
const std::set<std::string> SlideshowManager::s_extensions = {
    ".jpg",
    ".png"
};

// Private functions

void SlideshowManager::ExtensionToLower(std::string &extension){
    int n = extension.length();
    for (int i = 0; i < n; i++){
        if (extension[i] >= 'A' && extension[i] <= 'Z')
            extension[i] += 32; 
    }
}

// Public functions

void SlideshowManager::SetOpenProg(const std::string &ImageCommand){
    SlideshowManager::s_openProg = ImageCommand;
}

bool SlideshowManager::GetFilesInPath(const std::string &path){
    int n = 0;
    m_paths.clear();
    if (!fs::exists(path))
        return false;
    for (const auto &x : fs::directory_iterator(path)){
        if (x.is_directory())
            continue;
        std::string extenstion = x.path().extension();
        ExtensionToLower(extenstion);
        if (s_extensions.find(extenstion) == s_extensions.end())
            continue;
        m_paths.push_back(x.path());
        n++;
    }
    syslog(LOG_INFO, "[SlideshowManager] Loaded %i images", n);
    return true;
}
void SlideshowManager::OpenAllImages(){
    for (const auto &path : m_paths){
        system(&(s_openProg + ' ' + path + " &")[0]);
    }
}
void SlideshowManager::ReloadImages(const std::string &path){
    GetFilesInPath(path);
    m_WindowManager->CloseAllWindows();
    OpenAllImages();
}