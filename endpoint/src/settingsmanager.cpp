#include "settingsmanager.h"

// Constructors

SettingsManager::SettingsManager(){
    m_settings = EndpointSettings("", false, "", 0);
}

// Public functions

EndpointSettings SettingsManager::GetSettings(){
    return m_settings;
}
void SettingsManager::SetSettings(const EndpointSettings &settings){
    m_settings = settings;
}