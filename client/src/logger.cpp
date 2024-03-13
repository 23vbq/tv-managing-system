#include "logger.h"

// Static variables

const LogLevel Logger::LOG_DEBUG = LogLevel{0, "DEBUG"};
const LogLevel Logger::LOG_INFO = LogLevel{1, "INFO"};
const LogLevel Logger::LOG_WARNING = LogLevel{2, "WARNING"};
const LogLevel Logger::LOG_ERROR = LogLevel{3, "ERROR"};

// Constructors

Logger::Logger(string path){
    // Initialize variables
    m_path = path;
    memset(m_timebuffer, 0, _TIMEDATE_LEN);
    // Open log file
    m_fs.open(m_path, ios::app);
    if (!m_fs.is_open()){
        throw "Cannot open log file!";
    }
    // Write info
    m_fs<<"<======="<<GetTime()<<"=======>\n";
    Log(LOG_INFO, "Started logger");
}
Logger::~Logger(){
    Log(LOG_INFO, "Closing logger");
    m_fs.close();
}

// Private functions

char* Logger::GetTime(){
    m_unixtime = time(NULL);
    strftime(m_timebuffer, _TIMEDATE_LEN, "%H:%M:%S %d.%m.%Y", localtime(&m_unixtime));
    return m_timebuffer;
}

// Public functions

void Logger::Log(const LogLevel& level, string message){
    m_fs<<GetTime()<<" ["<<level.name<<"] "<<message<<"\n";
}