#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "serializer.h"
#include "clientsocketqt.h"

#include "connectwindow.h"

#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void ReloadEndpointListView();
    void SetCwPtr(ConnectWindow*);
private slots:
    void on_actionConnect_to_triggered();

    void on_actionDisconnect_triggered();

private:
    void OpenConnectWindow();
    void CloseConnectWindow();

    void LoadEndpointListView(std::string&);
    void LoadEndpointSettings(std::string&);

    Ui::MainWindow *ui;
    ConnectWindow *cw;
signals:
    void closed();
protected:
    void closeEvent(QCloseEvent*);
};
#endif // MAINWINDOW_H
