#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QMetaObject>

#include <QFile>
#include <Logger.hpp>
#include <gui/resources/lang/lang.hpp>

#include <MotionManagerImpl.hpp>


using namespace PROGRAM_NAMESPACE;

MainWindow::MainWindow(QWidget *parent) :
    UnmovableWindow(parent), ui(new Ui::MainWindow),
    errorManager(nullptr), galilCNInspector(nullptr) {

    traceEnter;

    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(this->width(), this->height());

    this->initDevices();
    this->initPanels();
    this->setupSignalsAndSlots();

    Settings::instance();

//    inspector.startProcess();

//    errorManager.subscribeObject(inspector);


    this->setupStyleSheets();

    QTimer::singleShot(1000, this, &MainWindow::startDevices);

    traceExit;

}

MainWindow::~MainWindow() {

    traceEnter;

    delete ui;

    traceExit;

}

void MainWindow::setupSignalsAndSlots() const {

    traceEnter;

    connect(ui->pbRefreshStyle, &QPushButton::clicked, this, &MainWindow::setupStyleSheets);
    connect(ui->pb, &QPushButton::clicked, []() {
        MotionManager::Ptr t = new MotionManagerImpl<DeviceKey::GALIL_CN>();
        t->moveX(10);
        delete t;
    });

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

    using namespace PROGRAM_NAMESPACE;

    int widgetsCount = ui->stackedWidget->count();
    for (int i=0; i<widgetsCount; ++i) {

        QWidget* current = ui->stackedWidget->widget(i);
        if (MotionFrame::Ptr mf = current->findChild<MotionFrame::Ptr>(QString(), Qt::FindDirectChildrenOnly)) {
            connect(galilCNInspector.data(), &GalilCNInspector::statusSignal, [mf](GalilCNStatusBean bean) {
                QMetaObject::invokeMethod(mf, "updateUI", Qt::QueuedConnection, Q_ARG(const mibdv::MotionBean&, MotionBean(bean)));
            });
            mf->init();
        }

    }

    traceExit;

}

void MainWindow::initDevices() {

    traceEnter;

    Settings& s = Settings::instance();

    errorManager.reset(new ErrorManager());

    if (s.getMachineCNType() == DeviceKey::GALIL_CN) {

        galilCNInspector.reset(new GalilCNInspector());
        galilCNInspectorThread.reset(new QThread());

        connect(galilCNInspectorThread.data(), &QThread::started, galilCNInspector.data(), &GalilCNInspector::startProcess);
        connect(galilCNInspector.data(), &GalilCNInspector::processStopSignal, galilCNInspectorThread.data(), &QThread::quit);

        connect(galilCNInspector.data(), &GalilCNInspector::statusSignal, this, &MainWindow::galilCNStatusUpdateSignal);

        connect(galilCNInspectorThread.data(), &QThread::finished, galilCNInspector.data(), &GalilCNInspector::deleteLater);
        connect(galilCNInspectorThread.data(), &QThread::finished, galilCNInspectorThread.data(), &QThread::deleteLater);

        galilCNInspector.data()->moveToThread(galilCNInspectorThread.data());

    }

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

void MainWindow::startDevices() {

    traceEnter;

    galilCNInspectorThread.data()->start();

    traceExit;

}

