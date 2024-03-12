#ifndef LOGGER_H
#define LOGGER_H

#include "loglevel.h"

#include <fstream>

using namespace std;

class Logger
{
public:
    static const LogLevel LOG_DEBUG; // Debug
    static const LogLevel LOG_INFO; // Info
    static const LogLevel LOG_WARNING; // Warning
    static const LogLevel LOG_ERROR; // Error
private:
    ofstream m_fs;
public:
    Logger();
    ~Logger();

    void Log(string);
};

#endif