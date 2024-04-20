#include "logger.h"

// Static variables

std::string Logger::s_path = "";
QtMessageHandler Logger::s_orginalMessageHandler = nullptr;

// Constructors

Logger::Logger(std::string path){
    s_path = path;
}

// Private functions

void Logger::logToFile(QtMsgType type, const QMessageLogContext& context, const QString& msg){
    QString message = qFormatLogMessage(type, context, msg);
    static FILE* f = fopen(&s_path[0], "a");
    if (f == nullptr){
        QMessageBox msgBox;
        msgBox.critical(nullptr, "Log", "Cannot open log file!");
        exit(1);
    }
    fprintf(f, "%s\n", qPrintable(message));
    fflush(f);
    /*if (orginalMessageHandler)
        *orginalMessageHandler(type, context, msg);*/
}

// Public functions
void Logger::Initialize(){
    s_orginalMessageHandler = qInstallMessageHandler(logToFile);
    qInfo("Initialized logger");
}
