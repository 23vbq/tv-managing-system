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
};

#endif
