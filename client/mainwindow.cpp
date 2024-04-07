#include "mainwindow.h"
#include "./ui_mainwindow.h"

extern ClientSocketQt* m_ClientSocket;
extern EndpointManager* m_EndpointManager;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(1280, 741));
    ClearEndpointSettings();
    ReloadEndpointListView();

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::ReloadEndpointListView);
    connect(ui->epSavePushButton, &QPushButton::clicked, this, &MainWindow::SaveEndpointSettings);

    connect(ui->endpointListWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::LoadEndpointSettings);

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

void MainWindow::ReloadEndpointListView(){
    ui->endpointListWidget->clear();
    if (!m_ClientSocket->IsConnected())
        return;
    m_ClientSocket->Send("GETEPLS");
    std::string result;
    if (!m_ClientSocket->Read(result)){
        m_msg.critical(this, "Connection", "Cannot load endpoint list");
        return;
    }
    LoadEndpointListView(result);
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

void MainWindow::LoadEndpointListView(std::string& serializedEndpointList){
    size_t n;
    std::vector<std::string> names;
    Serializer sr(serializedEndpointList);
    try{
        n = sr.DeserializeNext<size_t>();
    } catch (const char* e){
        m_msg.critical(this, "Serializer", e);
        return;
    }
    for (size_t i = 0; i < n; i++){
        std::string name;
        try{
            name = sr.DeserializeNext();
        } catch (const char* e){
            m_msg.critical(this, "Serializer", e);
            return;
        }
        names.push_back(name);
        ui->endpointListWidget->addItem(QString::fromStdString(name));
    }
}
void MainWindow::LoadEndpointSettings(){
    EndpointSettings ep; // Pointer?
    std::string name = ui->endpointListWidget->currentIndex().data().toString().toStdString();
    if (!m_EndpointManager->GetEndpointSettings(name, ep)){
        m_msg.critical(this, "Connection", "Cannot load endpoint settings!");
        return;
    }
    ui->epNameLabel->setText(QString::fromStdString(ep.name));
    ui->epLocalCfgCheckBox->setCheckState((ep.localcfg ? Qt::CheckState::Checked : Qt::CheckState::Unchecked));
    ui->epDirLineEdit->setText(QString::fromStdString(ep.dir));
    ui->epShowtimeSpinBox->setValue(ep.showtime);
    ui->epSettings->setEnabled(true);
}
void MainWindow::ClearEndpointSettings(){
    ui->epNameLabel->setText("");
    ui->epLocalCfgCheckBox->setCheckState(Qt::CheckState::Unchecked);
    ui->epDirLineEdit->setText("");
    ui->epShowtimeSpinBox->setValue(0);
    ui->epSettings->setEnabled(false);
}
void MainWindow::SaveEndpointSettings(){
    m_EndpointManager->SetLocalCfg((ui->epLocalCfgCheckBox->checkState() == Qt::CheckState::Checked ? true : false)); // ui->epLocalCfgCheckBox->isChecked()
    QString d = ui->epDirLineEdit->text();
    std::string e = d.toStdString();
    m_EndpointManager->SetDir(e);
    m_EndpointManager->SetShowtime(ui->epShowtimeSpinBox->value());
    if (!m_EndpointManager->SaveEndpointSettings()){
        m_msg.critical(this, "Connection", "Cannot save endpoint settings!");
        return;
    }
    m_msg.information(this, "Endpoint", "Successfully saved endpoint settings.");
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
    ReloadEndpointListView();
    ClearEndpointSettings();
}

