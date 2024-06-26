#include "connectwindow.h"
#include "./ui_connectwindow.h"

extern ClientSocketQt* m_ClientSocket;

ConnectWindow::ConnectWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ConnectWindow)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(400,120));

    QLineEdit::connect(ui->portLineEdit, &QLineEdit::textEdited, this, &ConnectWindow::PortLineEditTextHandler); // Input mask for port (allow only numbers)
    connect(ui->connectButton, &QPushButton::clicked, this, &ConnectWindow::ConnectBtnHandler); // Connect button action
    connect(this, &ConnectWindow::enterPressed, this, &ConnectWindow::ConnectBtnHandler); // Connect action on enter pressed
}

ConnectWindow::~ConnectWindow()
{
    //this->parent->setEnabled(true);
    delete ui;
}

// Public functions

bool ConnectWindow::Connect(std::string addr, unsigned int port){
    QMessageBox msg;
    if (!m_ClientSocket->Connect(addr, port)){
        msg.critical(this, "Connection", "Cannot connect to: " + QString::fromStdString(addr) + ':' + QString::number(port));
        return false;
    }
    msg.information(this, "Connection", "Successfully connected");
    return true;
}

// Private functions

void ConnectWindow::ConnectBtnHandler(){
    bool ok = true;
    // Get input
    QString sAddr = ui->ipLineEdit->text();
    QString sPort = ui->portLineEdit->text();
    unsigned int port = sPort.toUInt(&ok);
    if (!ok){
        QMessageBox msg;
        msg.critical(this, "Connection", "Invalid address or port!");
        return;
    }
    // Try to connect
    if (Connect(sAddr.toStdString(), port))
        this->close();
}
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
void ConnectWindow::keyPressEvent(QKeyEvent* event){
    int key = event->key();
    if (key == Qt::Key_Enter || key == Qt::Key_Return)
        emit enterPressed();
}
