#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "gui/custom-widgets/MDListWidget.hpp"

using namespace PROGRAM_NAMESPACE;

void MainWindow::setupSignalsAndSlots() const {

    traceEnter;

    connect(ui->buttonRefreshStyle, &QPushButton::clicked, this, &MainWindow::setupStyleSheets);

    traceExit;

}

void MainWindow::setupStyleSheets() const {

    using namespace PROGRAM_NAMESPACE;

    traceEnter;

    // TODO NIC 23/10/2018 - gestire fogli di stile
    QApplication* app = static_cast<QApplication*>(QApplication::instance());

    QFile file(QString(STYLESHEET_PATH)+"geometry.qss");
    file.open(QFile::ReadOnly);
    QByteArray content = file.readAll();

//    app->setStyleSheet(content);

    QFile file2(QString(STYLESHEET_PATH)+"theme1.qss");
    file2.open(QFile::ReadOnly);
    content += file2.readAll();

    app->setStyleSheet(content);

    ui->checkBox_5->setIsOn(!ui->checkBox_5->getIsOn());

    traceExit;

}

MainWindow::MainWindow(QWidget *parent) :
    UnmovableWindow(parent), ui(new Ui::MainWindow) {

    traceEnter;

    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(this->width(), this->height());
    this->setupSignalsAndSlots();

    QListWidgetItem* item = new QListWidgetItem();
    QListWidgetItem* item2 = new QListWidgetItem();
    QListWidgetItem* item3 = new QListWidgetItem();
    ui->listWidget->addItem(item);
    ui->listWidget->addItem(item2);
    ui->listWidget->addItem(item3);
    ui->listWidget->setItemWidget(item, new MDCustomItem("prova", ":/mibdv/settings"));
    ui->listWidget->setItemWidget(item2, new MDCustomItem("prova2", ":/mibdv/expand-less"));
    ui->listWidget->setItemWidget(item3, new MDCustomItem("prova3", ":/mibdv/expand-more"));


    this->setupStyleSheets();

    traceExit;

}

MainWindow::~MainWindow() {

    traceEnter;

    delete ui;

    traceExit;

}
