#ifndef ENDPOINT_MANAGER_H
#define ENDPOINT_MANAGER_H

#include "endpointsettings.h"
#include "clientsocketqt.h"
#include "serializer.h"

class EndpointManager{
private:
    EndpointSettings m_settings;

public:
    bool GetEndpointSettings(std::string&, EndpointSettings&);
    bool SaveEndpointSettings();

    void SetLocalCfg(const bool&);
    void SetDir(const std::string&);
    void SetShowtime(const unsigned int&);
};

#endif
