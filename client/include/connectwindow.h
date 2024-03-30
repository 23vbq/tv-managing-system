#ifndef CONNECTWINDOW_H
#define CONNECTWINDOW_H

#include <QMainWindow>
#include <QPushButton>

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
    Ui::ConnectWindow *ui;
};

#endif