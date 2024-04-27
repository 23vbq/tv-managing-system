#include "mainwindow.h"
#include "connectwindow.h"
#include "authwindow.h"
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

    // Create windows
    MainWindow mainWindow;
    ConnectWindow connectWindow;
    AuthWindow authWindow;
    // Pass data to windows
    mainWindow.SetCwPtr(&connectWindow);
    mainWindow.SetAwPtr(&authWindow);
    QMainWindow* authWndOtherList[] = {&mainWindow, &connectWindow};
    authWindow.SetOtherWindowsList(2, authWndOtherList);
    m_ClientSocket->SetAwPtr(&authWindow);
    // Show main window
    mainWindow.show();
    // Initialize logger
    log.Initialize();

    return a.exec();
}
