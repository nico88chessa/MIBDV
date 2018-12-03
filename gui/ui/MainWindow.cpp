#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QMetaObject>

#include <QFile>
#include <Logger.hpp>
#include <gui/resources/lang/lang.hpp>

#include <MotionManagerImpl.hpp>
#include <galil/GalilCNController.hpp>
#include <galil/GalilPLCController.hpp>


using namespace PROGRAM_NAMESPACE;

MainWindow::MainWindow(QWidget *parent) :
    UnmovableWindow(parent), ui(new Ui::MainWindow),
    errorManager(nullptr),
    galilCNInspector(nullptr),
    galilPLCInspector(nullptr),
    ioManager(nullptr),
    motionManager(nullptr),
    cn(nullptr),
    plc(nullptr) {

    traceEnter;

    this->initDevices();

    this->setupUi();
    this->setupStyleSheets();

    this->setupSignalsAndSlots();

    QTimer::singleShot(1000, this, &MainWindow::startDevices);

    traceExit;

}

MainWindow::~MainWindow() {

    traceEnter;

    stopDevices();

    delete ui;

    traceExit;

}

void MainWindow::setupUi() {

    traceEnter;

    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(this->width(), this->height());

    this->setupUiPanels();

    traceExit;

}

void MainWindow::setupSignalsAndSlots() const {

    traceEnter;

    connect(ui->pbRefreshStyle, &QPushButton::clicked, this, &MainWindow::setupStyleSheets);
    /*connect(ui->pb, &QPushButton::clicked, []() {
        MotionManager::Ptr t = new MotionManagerImpl<DeviceKey::GALIL_CN>();
        t->moveX(10);
        delete t;
    });*/

    // left panel

    auto listItem = ui->listItem;
    connect(listItem, &MDListWidget::itemClicked, [&, listItem](QListWidgetItem* item) {
        auto itemWidget = static_cast<MDCustomItem::Ptr>(listItem->itemWidget(item));
        if (itemWidget == nullptr)
            return;
        QString text = itemWidget->text();
        if (QString(MAINWINDOW_MDCUSTOMITEM_MOTION).compare(text) == 0)
            ui->stackedWidget->setCurrentWidget(ui->pageMotion);
        else if (QString(MAINWINDOW_MDCUSTOMITEM_IO).compare(text) == 0)
            ui->stackedWidget->setCurrentWidget(ui->pageIO);

    });

    // content panel

    int widgetsCount = ui->stackedWidget->count();
    for (int i=0; i<widgetsCount; ++i) {

        QWidget* current = ui->stackedWidget->widget(i);
        if (auto mf = current->findChild<MotionFrame::Ptr>(QString(), Qt::FindDirectChildrenOnly)) {

            if (Settings::instance().getMachineCNType() == DeviceKey::GALIL_CN) {
                connect(galilCNInspector.data(), &GalilCNInspector::statusSignal, [mf](GalilCNStatusBean bean) {
                    // faccio una funziona lambda per convertire da GalilCNStatusBean a MotionBean
                    QMetaObject::invokeMethod(mf, "updateUI", Qt::QueuedConnection, Q_ARG(const mibdv::MotionBean&, MotionBean(bean)));
                });
            }
        }

    }


    traceExit;

}

void MainWindow::setupUiPanels() {

    traceEnter;

    setupUiLeftPanel();
    setupUiContentPanel();

    traceExit;

}

void MainWindow::setupUiLeftPanel() {

    traceEnter;

    MDCustomItem::Ptr alert = new MDCustomItem(MAINWINDOW_MDCUSTOMITEM_ALERT, ":/mibdv/alert");
    MDCustomItem::Ptr io = new MDCustomItem(MAINWINDOW_MDCUSTOMITEM_IO, ":/mibdv/io");
    MDCustomItem::Ptr motion = new MDCustomItem(MAINWINDOW_MDCUSTOMITEM_MOTION, ":mibdv/motion");

    QListWidgetItem* alertItem = new QListWidgetItem(ui->listItem);
    QListWidgetItem* ioItem = new QListWidgetItem(ui->listItem);
    QListWidgetItem* motionItem = new QListWidgetItem(ui->listItem);

    ui->listItem->addItem(alertItem);
    ui->listItem->addItem(ioItem);
    ui->listItem->addItem(motionItem);
    ui->listItem->setItemWidget(alertItem, alert);
    ui->listItem->setItemWidget(ioItem, io);
    ui->listItem->setItemWidget(motionItem, motion);

    traceExit;

}

void MainWindow::setupUiContentPanel() {

    traceEnter;

    using namespace PROGRAM_NAMESPACE;

    int widgetsCount = ui->stackedWidget->count();
    for (int i=0; i<widgetsCount; ++i) {

        QWidget* current = ui->stackedWidget->widget(i);
        if (auto mf = current->findChild<MotionFrame::Ptr>(QString(), Qt::FindDirectChildrenOnly)) {

            mf->setupLogic(this->motionManager);

        }

    }

    traceExit;

}

void MainWindow::initDevices() {

    traceEnter;

    Settings& s = Settings::instance();

    if (errorManager.isNull())
        errorManager.reset(new ErrorManager());

    // metto prima gli inspector perche' sono utilizzati da motion manager e iomanager per gestire i segnali
    if (s.getMachineCNType() == DeviceKey::GALIL_CN)
        this->initGalilCNInspector();

    if (s.getMachinePLCType() == DeviceKey::GALIL_PLC)
        this->initGalilPLCInspector();

    this->initMotionManager();
    this->initIOManager();

    traceExit;

}

void MainWindow::initMotionManager() {

    traceEnter;

    Settings& s = Settings::instance();

    if (s.getMachineCNType() == DeviceKey::GALIL_CN) {

        traceDebug() << "Utilizzo il CN Galil come motion manager";
        if (motionManager.isNull()) {

            if (cn.isNull()) {
                cn.reset(new GalilCNController());
                GalilCNController::Ptr cnPtr = static_cast<GalilCNController::Ptr>(cn.data());

                cnPtr->setupController(s.getGalilCNNumberDigitalInput(), s.getGalilCNNumberDigitalOutput(), s.getGalilCNNumberAnalogInput());
                if (cnPtr->connect(s.getGalilCNIpAddress()))
                    traceErr() << "Impossibile connettersi al CN";

            }

            motionManager.reset(new MotionManagerImpl<GalilCNController>(cn.staticCast<GalilCNController>()));

            // controllo che gli oggetti siano validi
            if (!(galilCNInspector.isNull() && motionManager.isNull())) {
                // TODO NIC 03/12/2018 - completare gli altri segnali
                connect(galilCNInspector.data(), &GalilCNInspector::powerOffSignal, motionManager.data(), &MotionManager::powerOffSignal);
                connect(galilCNInspector.data(), &GalilCNInspector::cycleOffSignal, motionManager.data(), &MotionManager::cycleOffSignal);
                connect(galilCNInspector.data(), &GalilCNInspector::axisXMotorOffSignal, motionManager.data(), &MotionManager::axisXMotorOffSignal);
            }

        }

    }

    traceExit;

}

void MainWindow::initIOManager() {

    traceEnter;

    if (ioManager.isNull())
        ioManager.reset(new IOManager());

    Settings& s = Settings::instance();

    if (s.getMachineCNType() == DeviceKey::GALIL_CN) {

        traceDebug() << "Inserisco il CN Galil ai dispositivi di IO";

        if (cn.isNull()) {

            cn.reset(new GalilCNController());
            GalilCNController::Ptr cnPtr = static_cast<GalilCNController::Ptr>(cn.data());
            cnPtr->setupController(s.getGalilCNNumberDigitalInput(), s.getGalilCNNumberDigitalOutput(), s.getGalilCNNumberAnalogInput());
            if (cnPtr->connect(s.getGalilCNIpAddress()))
                traceErr() << "Impossibile connettersi al CN";

        }

        ioManager->addDevice(DeviceKey::GALIL_CN, cn.staticCast<GalilCNController>());

    }

    if (s.getMachinePLCType() == DeviceKey::GALIL_PLC) {

        traceDebug() << "Inserisco il PLC Galil ai dispositivi di IO";
        if (plc.isNull()){

            plc.reset(new GalilPLCController());
            GalilPLCController::Ptr plcPtr = static_cast<GalilPLCController::Ptr>(plc.data());
            plcPtr->setupController(s.getGalilPLCNumberDigitalInput(), s.getGalilPLCNumberDigitalOutput(), s.getGalilPLCNumberAnalogInput());
            if (!plcPtr->connect(s.getGalilPLCIpAddress()))
                traceErr() << "Impossibile connettersi al PLC";

        }

        ioManager->addDevice(DeviceKey::GALIL_PLC, plc.staticCast<GalilPLCController>());

    }

    traceExit;

}

void MainWindow::initGalilCNInspector() {

    traceEnter;

    QThread* galilCNInspectorThread = new QThread();

    galilCNInspector.reset(new GalilCNInspector());

    connect(galilCNInspectorThread, &QThread::started, galilCNInspector.data(), &GalilCNInspector::startProcess);
    connect(galilCNInspector.data(), &GalilCNInspector::processStopSignal, galilCNInspectorThread, &QThread::quit);

    connect(galilCNInspector.data(), &GalilCNInspector::statusSignal, this, &MainWindow::galilCNStatusUpdateSignal);

    connect(galilCNInspectorThread, &QThread::finished, galilCNInspector.data(), &GalilCNInspector::deleteLater);
    connect(galilCNInspectorThread, &QThread::finished, galilCNInspectorThread, &QThread::deleteLater);

    galilCNInspector.data()->moveToThread(galilCNInspectorThread);

    errorManager->subscribeObject(*galilCNInspector);

    traceExit;

}

void MainWindow::initGalilPLCInspector() {

    traceEnter;

    QThread* galilPLCInspectorThread = new QThread();

    galilPLCInspector.reset(new GalilPLCInspector());

    connect(galilPLCInspectorThread, &QThread::started, galilPLCInspector.data(), &GalilPLCInspector::startProcess);
    connect(galilPLCInspector.data(), &GalilPLCInspector::processStopSignal, galilPLCInspectorThread, &QThread::quit);

    connect(galilPLCInspector.data(), &GalilPLCInspector::statusSignal, this, &MainWindow::galilPLCStatusUpdateSignal);

    connect(galilPLCInspectorThread, &QThread::finished, galilPLCInspector.data(), &GalilPLCInspector::deleteLater);
    connect(galilPLCInspectorThread, &QThread::finished, galilPLCInspectorThread, &QThread::deleteLater);

    galilPLCInspector.data()->moveToThread(galilPLCInspectorThread);

    errorManager->subscribeObject(*galilPLCInspector);


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

    Settings& s = Settings::instance();

    if (s.getMachineCNType() == DeviceKey::GALIL_CN) {
        QThread* galilCNThread = galilCNInspector->thread();
        galilCNThread->start();
    }

    if (s.getMachinePLCType() == DeviceKey::GALIL_PLC) {
        QThread* galilPLCThread = galilPLCInspector->thread();
        galilPLCThread->start();
    }

    traceExit;

}

void MainWindow::stopDevices() {

    traceEnter;

    Settings& s = Settings::instance();

    if (s.getMachineCNType() == DeviceKey::GALIL_CN)
        galilCNInspector->stopProcess();

    if (s.getMachinePLCType() == DeviceKey::GALIL_PLC)
        galilPLCInspector->stopProcess();


    traceExit;

}

