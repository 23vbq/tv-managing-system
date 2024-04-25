#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QMainWindow>

namespace Ui {
class AuthWindow;
}

class AuthWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AuthWindow(QWidget *parent = nullptr);
    ~AuthWindow();

private:
    Ui::AuthWindow *ui;
};

#endif // AUTHWINDOW_H
