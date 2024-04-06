#include "mainwindow.h"
#include "serializer.h"
#include "endpointmanager.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
#include <QTemporaryDir>

#include "logger.h"
#include "clientsocketqt.h"

ClientSocketQt* m_ClientSocket;
EndpointManager* m_EndpointManager;

int main(int argc, char *argv[])
{
    // Construct logger class
    QTemporaryDir tempDir;
    if (!tempDir.isValid()){
        QMessageBox msg;
        msg.critical(nullptr, "Filesystem", "Cannot create temp directory");
        return 1;
    }

    Logger log(tempDir.path().toStdString() + "\\syslog");

    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "client_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // Initialize client socket
    m_ClientSocket = new ClientSocketQt(nullptr);

    // Initialize endpoint manager
    m_EndpointManager = new EndpointManager();

    MainWindow mainWindow;
    ConnectWindow connectWindow;
    mainWindow.SetCwPtr(&connectWindow);
    mainWindow.show();
    // connectWindow.show();
    // Initialize logger
    log.Initialize();
    /*ClientSocketQt cs(nullptr);
    QMessageBox conMsg;
    if (cs.Connect("192.168.121.132", (unsigned int)5555))
        conMsg.information(nullptr, "Client Socket", "Successfully connected!");
    else
        conMsg.critical(nullptr, "Client Socket", "Unable to connect!");*/
    //w.ReloadEndpointListView("\"2\"\"4:Test\"\"5:kotek\"");
    // w.ReloadEndpointListView("2\"3:kot\"\"6:piesek\"");
    return a.exec();
}
