#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include <syslog.h>

#include "endpointsettings.h"

class SettingsManager{
private:
    EndpointSettings m_settings;
public:
    SettingsManager();
    ~SettingsManager();

    void SetSettings(const EndpointSettings&);
};

#endif