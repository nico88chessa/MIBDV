#include "MainWindow.hpp"
#include "ui_MainWindow.h"

using namespace PROGRAM_NAMESPACE;

void MainWindow::setupSignalsAndSlots() const {

    traceEnter;

    connect(ui->buttonRefreshStyle, &QPushButton::clicked, this, &MainWindow::setupStyleSheets);

    traceExit;

}

void MainWindow::initWidgets() {


    MDCustomItem::Ptr alerts = new MDCustomItem("Alerts", ":/mibdv/alert");
    MDCustomItem::Ptr alerts2 = new MDCustomItem("Alerts", ":/mibdv/pan");

    QListWidgetItem* item = new QListWidgetItem();
    ui->listItem->addItem(item);
    ui->listItem->setItemWidget(item, alerts);

    QListWidgetItem* item2 = new QListWidgetItem();
    ui->listItem->addItem(item2);
    ui->listItem->setItemWidget(item2, alerts2);

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

MainWindow::MainWindow(QWidget *parent) :
    UnmovableWindow(parent), ui(new Ui::MainWindow) {

    traceEnter;

    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(this->width(), this->height());

    this->initWidgets();
    this->setupSignalsAndSlots();

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
