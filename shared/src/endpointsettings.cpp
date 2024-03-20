#include "endpointsettings.h"

// Constructors

EndpointSettings::EndpointSettings(){
    name = "";
    localcfg = false;
    dir = "";
    showtime = 0;
}
EndpointSettings::EndpointSettings(std::string name, bool localcfg, std::string dir, unsigned int showtime){
    this->name = name;
    this->localcfg = localcfg;
    this->dir = dir;
    this->showtime = showtime;
}
EndpointSettings::EndpointSettings(const EndpointSettings& copy){
    name = copy.name;
    localcfg = copy.localcfg;
    dir = copy.dir;
    showtime = copy.showtime;
}