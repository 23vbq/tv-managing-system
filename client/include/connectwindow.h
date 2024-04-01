#ifndef CONNECTWINDOW_H
#define CONNECTWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QCloseEvent>
#include <QMessageBox>

#include "clientsocketqt.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ConnectWindow;
}
QT_END_NAMESPACE

class ConnectWindow : public QMainWindow
{
    Q_OBJECT

public:
    ConnectWindow(QWidget *parent = nullptr);
    ~ConnectWindow();

private:
    QWidget* parent;
    Ui::ConnectWindow *ui;

    void ConnectBtnHandler();
    void PortLineEditTextHandler();
signals:
    void closed();
protected:
    void closeEvent(QCloseEvent*);
};

#endif