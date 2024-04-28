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

    // Buttons
    connect(ui->loginButton, &QPushButton::clicked, this, &AuthWindow::LoginBtnHandler);
    connect(ui->showPassPushButton, &QPushButton::clicked, this, &AuthWindow::ShowPassBtnHandler);

    // Key
    connect(this, &AuthWindow::enterPressed, this, &AuthWindow::LoginBtnHandler);
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
    QString qkey = ui->passwdLineEdit->text();
    // Reset form
    ui->passwdLineEdit->setText("");
    ui->passwdLineEdit->setEchoMode(QLineEdit::Password);
    // Send and handle response
    m_ClientSocket->Send("AUTHK \2" + qkey.toStdString() + "\3");
    std::string result;
    qint64 res_size = m_ClientSocket->Read(result);
    QMessageBox msg;
    if (!res_size){
        msg.critical(this, "Authentication", "Authentication error");
        return;
    }
    if (result.find("SUCCESS") != std::string::npos){
        msg.information(this, "Authentication", "Successfully authenticated");
        this->close();
    } else{
        msg.critical(this, "Authentication", "Invalid key");
    }
}
void AuthWindow::ShowPassBtnHandler(){
    if (ui->passwdLineEdit->echoMode() == QLineEdit::Password)
        ui->passwdLineEdit->setEchoMode(QLineEdit::Normal);
    else
        ui->passwdLineEdit->setEchoMode(QLineEdit::Password);
}
void AuthWindow::SetDisabledOtherWindows(const bool& disable){
    for (size_t i = 0; i < otherWindowsList_size; i++){
        otherWindowsList[i]->setDisabled(disable);
    }
}

// Protected

void AuthWindow::closeEvent(QCloseEvent * event)
{
    QMainWindow::closeEvent(event);

    SetDisabledOtherWindows(false);

    if (event->isAccepted())
        emit closed();
}
void AuthWindow::keyPressEvent(QKeyEvent* event){
    int key = event->key();
    if (key == Qt::Key_Enter || key == Qt::Key_Return)
        emit enterPressed();
}
void AuthWindow::showEvent(QShowEvent* event){
    QMainWindow::showEvent(event);

    SetDisabledOtherWindows(true);
    ui->passwdLineEdit->setText("");
    ui->passwdLineEdit->setEchoMode(QLineEdit::Password);

    if (event->isAccepted())
        emit showed();
}
