#ifndef ENDPOINT_SETTINGS_H
#define ENDPOINT_SETTINGS_H

#include <string>

struct EndpointSettings{
    std::string name;
    bool localcfg; // Use local config instead of global
    std::string dir; // Directory of image files
    unsigned int showtime; // Time in seconds to show one 
};

#endif