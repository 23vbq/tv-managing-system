#include "settingsmanager.h"

// Constructors

SettingsManager::SettingsManager(){
    m_settings = EndpointSettings("", false, "", 0);
}

// Public functions

void SettingsManager::SetSettings(const EndpointSettings &settings){
    m_settings = settings;
}