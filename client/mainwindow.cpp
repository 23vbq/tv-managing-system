#include "mainwindow.h"
#include "./ui_mainwindow.h"

extern ClientSocketQt* m_ClientSocket;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(1280, 741));
    ReloadEndpointListView("\"2\"\"5:Test1\"\"6:Test12\"");

    connect(ui->pushButton, &QPushButton::clicked, [this](){
        ReloadEndpointListView("\"2\"\"7:Test123\"\"8:Test1234\"");
    });

    // cw = new ConnectWindow(this);
    // cw->show();
    // ReloadEndpointListView("\"2\"\"7:Test123\"\"8:Test1234\"");
}

MainWindow::~MainWindow()
{
    // delete cw;
    delete ui;
}

// Public functions

void MainWindow::ReloadEndpointListView(std::string serializedEndpointList){
    std::vector<std::string> names;
    Serializer sr(serializedEndpointList);
    size_t n = sr.DeserializeNext<size_t>();
    ui->endpointListWidget->clear();
    for (size_t i = 0; i < n; i++){
        std::string name = sr.DeserializeNext();
        names.push_back(name);
        ui->endpointListWidget->addItem(QString::fromStdString(name));
    }
}

void MainWindow::SetCwPtr(ConnectWindow* ptr){
    cw = ptr;
    connect(cw, &ConnectWindow::closed, this, &MainWindow::CloseConnectWindow);
}

// Private slots functions

void MainWindow::on_actionConnect_to_triggered()
{
    OpenConnectWindow();
}

// Private functions

void MainWindow::OpenConnectWindow(){
    this->setEnabled(false);
    cw->show();
}
void MainWindow::CloseConnectWindow(){
    this->setEnabled(true);
}

// Protected

void MainWindow::closeEvent(QCloseEvent * event)
{
    delete m_ClientSocket;
    QMainWindow::closeEvent(event);

    if (event->isAccepted())
        emit closed();
}

void MainWindow::on_actionDisconnect_triggered()
{
    m_ClientSocket->Disconnect();
}

