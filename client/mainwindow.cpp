#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ReloadEndpointListView("\"2\"\"5:Test1\"\"6:Test12\"");

    connect(ui->pushButton, &QPushButton::clicked, [this](){
        ReloadEndpointListView("\"2\"\"7:Test123\"\"8:Test1234\"");
    });

    cw = new ConnectWindow(this);
    cw->show();
}

MainWindow::~MainWindow()
{
    delete cw;
    delete ui;
}

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
