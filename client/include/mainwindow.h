#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "serializer.h"
#include "clientsocketqt.h"
#include "endpointmanager.h"

#include "connectwindow.h"
#include "authwindow.h"

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
    void SetAwPtr(AuthWindow*);
private slots:
    void on_actionConnect_to_triggered();

    void on_actionDisconnect_triggered();

    void on_actionReconnect_triggered();

private:
    void OpenConnectWindow();
    void CloseConnectWindow();

    void LoadEndpointListView(std::string&);
    void LoadEndpointSettings();
    void ClearEndpointSettings();
    void SaveEndpointSettings();

    Ui::MainWindow *ui;
    ConnectWindow *cw;
    AuthWindow *aw;
    QMessageBox m_msg;
signals:
    void closed();
protected:
    void closeEvent(QCloseEvent*);
};
#endif // MAINWINDOW_H
