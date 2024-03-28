#ifndef LOGGER_H
#define LOGGER_H

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>

#include <stdio.h>
#include <stdlib.h>

class Logger{
private:
    static const char* s_path;
    static QtMessageHandler s_orginalMessageHandler;

    static void logToFile(QtMsgType type, const QMessageLogContext& context, const QString& msg);
public:
    Logger(const char*);

    void Initialize();
};
    

#endif