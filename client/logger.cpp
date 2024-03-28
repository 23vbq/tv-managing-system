#include "logger.h"

// Static variables

const char* Logger::s_path = "";
QtMessageHandler Logger::s_orginalMessageHandler = nullptr;

// Constructors

Logger::Logger(const char* path){
    s_path = path;
}

// Private functions

void Logger::logToFile(QtMsgType type, const QMessageLogContext& context, const QString& msg){
    QString message = qFormatLogMessage(type, context, msg);
    static FILE* f = fopen(s_path, "a");
    if (f == nullptr){
        QMessageBox msgBox;
        msgBox.setText("Cannot open log file!");
        msgBox.exec();
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