#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "serializer.h"
#include "connectwindow.h"

#include <QMainWindow>
#include <QPushButton>

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

    void ReloadEndpointListView(std::string);

private:
    Ui::MainWindow *ui;
    ConnectWindow *cw;
};
#endif // MAINWINDOW_H
