#include "connectwindow.h"
#include "./ui_connectwindow.h"

ConnectWindow::ConnectWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ConnectWindow)
{
    /*this->parent = parent;
    if (parent != nullptr){
        parent->setEnabled(false);
    }*/
    ui->setupUi(this);
    this->setFixedSize(QSize(400,120));

    QLineEdit::connect(ui->portLineEdit, &QLineEdit::textEdited, this, &ConnectWindow::PortLineEditTextHandler);
}

ConnectWindow::~ConnectWindow()
{
    //this->parent->setEnabled(true);
    delete ui;
}

// Private functions

void ConnectWindow::PortLineEditTextHandler(){
    QString text = ui->portLineEdit->text();
    size_t n = text.length();
    for (size_t i = 0; i < n; i++){
        if (!text[i].isDigit()){
            text.erase(text.begin() + i--);
            n--;
        }
    }
    ui->portLineEdit->setText(text);
}

// Protected

void ConnectWindow::closeEvent(QCloseEvent * event)
{
    QMainWindow::closeEvent(event);

    if (event->isAccepted())
        emit closed();
}
