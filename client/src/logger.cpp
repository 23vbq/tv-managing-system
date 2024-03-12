#include "logger.h"

// Static variables

const LogLevel Logger::LOG_DEBUG = LogLevel{0, "DEBUG"};
const LogLevel Logger::LOG_INFO = LogLevel{1, "INFO"};
const LogLevel Logger::LOG_WARNING = LogLevel{2, "WARNING"};
const LogLevel Logger::LOG_ERROR = LogLevel{3, "ERROR"};

// Constructors

Logger::Logger(){
    m_fs.open("test/log", ios::app);
    if (!m_fs.is_open()){
        throw "Cannot open log file!";
    }
    // FIXME test
    Log("TEST");
}
Logger::~Logger(){
    Log("Closing logger");
    m_fs.close();
}

// Public functions

void Logger::Log(string message){
    m_fs<<message<<"\n";
}