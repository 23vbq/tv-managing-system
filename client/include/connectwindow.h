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

    bool Connect(std::string, unsigned int);

private:
    QWidget* parent;
    Ui::ConnectWindow *ui;

    void ConnectBtnHandler();
    void PortLineEditTextHandler();
signals:
    void closed();
    void enterPressed();
protected:
    void closeEvent(QCloseEvent*);
    void keyPressEvent(QKeyEvent*);
};

#endif
