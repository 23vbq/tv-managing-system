#include "connectwindow.h"
#include "./ui_connectwindow.h"

ConnectWindow::ConnectWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ConnectWindow)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(400,120));
}

ConnectWindow::~ConnectWindow()
{
    delete ui;
}