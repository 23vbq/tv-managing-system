#ifndef LOGGER_H
#define LOGGER_H

#define _TIMEDATE_LEN 20

#include "loglevel.h"

#include <fstream>
#include <ctime>
#include <string>
#include <cstring>

// using namespace std;

class Logger
{
public:
    static const LogLevel LOG_DEBUG; // Debug
    static const LogLevel LOG_INFO; // Info
    static const LogLevel LOG_WARNING; // Warning
    static const LogLevel LOG_ERROR; // Error
private:
    std::string m_path;
    std::ofstream m_fs;

    time_t m_unixtime;
    char m_timebuffer[_TIMEDATE_LEN];

    char* GetTime();
public:
    Logger(std::string);
    ~Logger();

    void Log(const LogLevel&, std::string);
};

#endif