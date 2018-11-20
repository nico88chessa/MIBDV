#include "MainWindow.hpp"
#include "ui_MainWindow.h"

using namespace PROGRAM_NAMESPACE;

MainWindow::MainWindow(QWidget *parent) :
    UnmovableWindow(parent), ui(new Ui::MainWindow),
    errorManager(nullptr), galilCNInspector(nullptr) {

    traceEnter;

    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(this->width(), this->height());

    this->initPanels();
    this->setupSignalsAndSlots();

    Settings& s = Settings::instance();

//    inspector.startProcess();

//    errorManager.subscribeObject(inspector);


    this->setupStyleSheets();

    traceExit;

}

MainWindow::~MainWindow() {

    traceEnter;

    delete ui;

    traceExit;

}

void MainWindow::setupSignalsAndSlots() const {

    traceEnter;

    connect(ui->buttonRefreshStyle, &QPushButton::clicked, this, &MainWindow::setupStyleSheets);

    traceExit;

}

void MainWindow::initPanels() {

    traceEnter;

    initLeftPanel();
    initContentPanel();

    traceExit;

}

void MainWindow::initLeftPanel() {

    traceEnter;

    MDCustomItem::Ptr alert = new MDCustomItem(MAINWINDOW_MDCUSTOMITEM_ALERT, ":/mibdv/alert");
    MDCustomItem::Ptr io = new MDCustomItem(MAINWINDOW_MDCUSTOMITEM_IO, ":/mibdv/io");
    MDCustomItem::Ptr motion = new MDCustomItem(MAINWINDOW_MDCUSTOMITEM_MOTION, ":mibdv/motion");

    QListWidgetItem* alertItem = new QListWidgetItem();
    QListWidgetItem* ioItem = new QListWidgetItem();
    QListWidgetItem* motionItem = new QListWidgetItem();

    ui->listItem->addItem(alertItem);
    ui->listItem->addItem(ioItem);
    ui->listItem->addItem(motionItem);
    ui->listItem->setItemWidget(alertItem, alert);
    ui->listItem->setItemWidget(ioItem, io);
    ui->listItem->setItemWidget(motionItem, motion);


    traceExit;

}

void MainWindow::initContentPanel() {

    traceEnter;

    traceExit;

}

void MainWindow::initDevices() {

    traceEnter;

    errorManager.reset(new ErrorManager());
    galilCNInspector.reset(new GalilCNInspector());

    errorManager->subscribeObject(*galilCNInspector);

    traceExit;

}

void MainWindow::startGalilCNInspector() {

    traceEnter;

    traceExit;

}

void MainWindow::stopGalilCNInspector() {

    traceEnter;

    traceExit;

}

void MainWindow::setupStyleSheets() const {

    using namespace PROGRAM_NAMESPACE;

    traceEnter;

    ui->top->layout()->setAlignment(Qt::AlignBottom);

    // TODO NIC 23/10/2018 - gestire fogli di stile
    QApplication* app = static_cast<QApplication*>(QApplication::instance());
    app->setStyleSheet(styleSheet());

    QFile mdGeomStyle(QString(STYLESHEET_PATH)+"geometry.qss");
    mdGeomStyle.open(QFile::ReadOnly);
    QByteArray mdGeomStr = mdGeomStyle.readAll();

    QFile appGeom(QString(STYLESHEET_PATH)+"app-geometry.qss");
    appGeom.open(QFile::ReadOnly);
    QByteArray geomStr = appGeom.readAll();

    QFile theme(QString(STYLESHEET_PATH)+"theme1.qss");
    theme.open(QFile::ReadOnly);
    QByteArray themeStr = theme.readAll();

    QString content = mdGeomStr + geomStr + themeStr;

    app->setStyleSheet(content);

    traceExit;

}

