#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QKeyEvent>

#include "clientsocketqt.h"

namespace Ui {
class AuthWindow;
}

class AuthWindow : public QMainWindow
{
    Q_OBJECT

public:
    AuthWindow(QWidget *parent = nullptr);
    ~AuthWindow();

private:
    Ui::AuthWindow *ui;

    void LoginBtnHandler();
signals:
    void closed();
    void enterPressed();
protected:
    void closeEvent(QCloseEvent*);
    void keyPressEvent(QKeyEvent*);
};

#endif // AUTHWINDOW_H
