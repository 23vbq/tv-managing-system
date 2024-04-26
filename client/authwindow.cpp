#include "authwindow.h"
#include "./ui_authwindow.h"

#include "clientsocketqt.h"

extern ClientSocketQt* m_ClientSocket;

AuthWindow::AuthWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AuthWindow)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(400,120));

    connect(ui->loginButton, &QPushButton::clicked, this, &AuthWindow::LoginBtnHandler);
}

AuthWindow::~AuthWindow()
{
    delete ui;
}

// Public functions

void AuthWindow::SetOtherWindowsList(size_t n, QMainWindow**arr){
    this->otherWindowsList_size = n;
    this->otherWindowsList = arr;
}

// Private functions

void AuthWindow::LoginBtnHandler(){

}
