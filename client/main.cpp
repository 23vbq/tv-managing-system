#include "mainwindow.h"
#include "serializer.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>

#include "logger.h"

int main(int argc, char *argv[])
{
    // Construct logger class
    Logger log("E:\\programowanie\\cpp\\tv-managing-system\\client\\test\\syslog");
    Serializer sr;
    sr.AddValue((std::string)"Test");
    std::string s = sr.Serialize();
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
    MainWindow w;
    w.show();
    log.Initialize();
    //w.ReloadEndpointListView("\"2\"\"4:Test\"\"5:kotek\"");
    // w.ReloadEndpointListView("2\"3:kot\"\"6:piesek\"");
    return a.exec();
}
