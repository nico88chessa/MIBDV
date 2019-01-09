#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QMetaObject>

#include <QFile>
#include <Logger.hpp>
#include <gui/resources/lang/lang.hpp>

#include <MotionManagerImpl.hpp>

#include <galil/GalilCNController.hpp>
#include <galil/GalilCNInspector.hpp>
#include <galil/GalilCNConnectionWatcher.hpp>

#include <galil/GalilPLCController.hpp>
#include <galil/GalilPLCInspector.hpp>
#include <galil/GalilPLCConnectionWatcher.hpp>


using namespace PROGRAM_NAMESPACE;

MainWindow::MainWindow(QWidget *parent) :
    UnmovableWindow(parent), ui(new Ui::MainWindow),
    errorManager(nullptr),
    motionInspector(nullptr),
    galilPLCInspector(nullptr),
    ioManager(nullptr),
    motionManager(nullptr),
    cn(nullptr),
    plc(nullptr),
    cnConnectionWatcher(nullptr),
    plcConnectionWatcher(nullptr) {

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

    // TODO NIC 06/12/2018 - verificare se funziona
    motionInspector->thread()->wait();
    galilPLCInspector->thread()->wait();

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
        if (auto&& mf = current->findChild<MotionFrame::Ptr>(QString(), Qt::FindDirectChildrenOnly)) {

            connect(this, &MainWindow::motionStatusUpdateSignal, [mf](const QVariant& bean) {
                auto motionBean = MotionBean(bean.value<GalilCNStatusBean>());
                QMetaObject::invokeMethod(mf, "updateMotionBean", Qt::QueuedConnection,
                                          Q_ARG(const mibdv::MotionBean&, motionBean));

            });

            connect(this, &MainWindow::ioStatusUpdateSignal, [mf](auto a, auto b, auto c) {
                Q_UNUSED(b);
                Q_UNUSED(c);
                QMetaObject::invokeMethod(mf, "updateDigitalInputStatus", Qt::QueuedConnection,
                                          Q_ARG(const mibdv::DigitalInputStatus&, a));

            });

        } else if (auto&& iof = current->findChild<IOFrame::Ptr>(QString(), Qt::FindDirectChildrenOnly)) {
                connect(this, &MainWindow::ioStatusUpdateSignal, [iof](auto a, auto b, auto c) {
                    QMetaObject::invokeMethod(iof, "updateDigitalIOStatus", Qt::QueuedConnection,
                                              Q_ARG(const mibdv::DigitalInputStatus&, a),
                                              Q_ARG(const mibdv::DigitalOutputStatus&, b),
                                              Q_ARG(const mibdv::AnalogInputStatus&, c));
                });

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

    MDCustomItem::Ptr alert = new MDCustomItem(MAINWINDOW_MDCUSTOMITEM_ALERT, ":/icons/black-theme/add_alert");
    MDCustomItem::Ptr io = new MDCustomItem(MAINWINDOW_MDCUSTOMITEM_IO, ":/icons/black-theme/input");
    MDCustomItem::Ptr motion = new MDCustomItem(MAINWINDOW_MDCUSTOMITEM_MOTION, ":icons/black-theme/pan_tool");

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

            mf->setupDevices(this->motionManager, this->ioManager);

        } else if (auto iof = current->findChild<IOFrame::Ptr>(QString(), Qt::FindDirectChildrenOnly)) {

            iof->setupDevices(this->ioManager);

        }

    }

    traceExit;

}

void MainWindow::initDevices() {

    traceEnter;

    Settings& s = Settings::instance();

    if (errorManager.isNull())
        errorManager.reset(new ErrorManager());

    this->initIOInspector();

    if (ioManager.isNull()) {
        ioManager.reset(new IOManager());

        connect(ioInspector.data(), &IOInspector::powerOnSignal, ioManager.data(), &IOManager::powerOnSignal);
        connect(ioInspector.data(), &IOInspector::powerOffSignal, ioManager.data(), &IOManager::powerOffSignal);
        connect(ioInspector.data(), &IOInspector::cycleOnSignal, ioManager.data(), &IOManager::cycleOnSignal);
        connect(ioInspector.data(), &IOInspector::cycleOffSignal, ioManager.data(), &IOManager::cycleOffSignal);

    }

    // avvio il motion inspector (galil cn inspector)
    this->initMotionInspector();

    // avvio il plc inspector (galil plc inspector)
    this->initGalilPLCInspector();

    // inizializzo il motion manager
    if (s.getMachineCNType() == DeviceKey::GALIL_CN) {

        // a questo punto creo il cn galil
        if (cn.isNull()) {

            cn.reset(new GalilCNController());
            GalilCNController::Ptr cnPtr = static_cast<GalilCNController::Ptr>(cn.data());
            cnPtr->setupController(s.getGalilCNNumberDigitalInput(), s.getGalilCNNumberDigitalOutput(), s.getGalilCNNumberAnalogInput());

        }

        // lo aggiungo all'io manager
        traceDebug() << "Inserisco il CN Galil ai dispositivi di IO";
        ioManager->addDevice(DeviceKey::GALIL_CN, cn.staticCast<GalilCNController>());

        // e lo aggancio al motion manager
        if (motionManager.isNull()) {

            motionManager.reset(new MotionManagerImpl<GalilCNController>(cn.staticCast<GalilCNController>()));
            traceDebug() << "Utilizzo il CN Galil come motion manager";

            // TODO NIC 03/12/2018 - completare gli altri segnali
            // connect(motionInspector.data(), &AbstractMotionInspector::powerOffSignal, motionManager.data(), &MotionManager::powerOffSignal);
            // connect(motionInspector.data(), &AbstractMotionInspector::cycleOffSignal, motionManager.data(), &MotionManager::cycleOffSignal);

            connect(motionInspector.data(), &AbstractMotionInspector::axisXMotorOffSignal, motionManager.data(), &MotionManager::axisXMotorOffSignal);
            connect(motionInspector.data(), static_cast<void (AbstractMotionInspector::*)()>(&AbstractMotionInspector::axisXMotionStopSignal),
                    motionManager.data(), static_cast<void (MotionManager::*)()>(&MotionManager::axisXMotionStopSignal));
            connect(motionInspector.data(), static_cast<void (AbstractMotionInspector::*)(MotionStopCode, QString)>(&AbstractMotionInspector::axisXMotionStopSignal),
                    motionManager.data(), static_cast<void (MotionManager::*)(MotionStopCode, QString)>(&MotionManager::axisXMotionStopSignal));
            connect(motionInspector.data(), &AbstractMotionInspector::axisXForwardLimitSignal, motionManager.data(), &MotionManager::axisXForwardLimitSignal);
            connect(motionInspector.data(), &AbstractMotionInspector::axisXBackwardLimitSignal, motionManager.data(), &MotionManager::axisXBackwardLimitSignal);
            connect(motionInspector.data(), &AbstractMotionInspector::axisXHomeInProgressStartSignal, motionManager.data(), &MotionManager::axisXHomeInProgressStartSignal);
            connect(motionInspector.data(), &AbstractMotionInspector::axisXHomeInProgressStopSignal, motionManager.data(), &MotionManager::axisXHomeInProgressStopSignal);

            connect(motionInspector.data(), &AbstractMotionInspector::axisYMotorOffSignal, motionManager.data(), &MotionManager::axisYMotorOffSignal);
            connect(motionInspector.data(), static_cast<void (AbstractMotionInspector::*)()>(&AbstractMotionInspector::axisYMotionStopSignal),
                    motionManager.data(), static_cast<void (MotionManager::*)()>(&MotionManager::axisYMotionStopSignal));
            connect(motionInspector.data(), static_cast<void (AbstractMotionInspector::*)(MotionStopCode, QString)>(&AbstractMotionInspector::axisYMotionStopSignal),
                    motionManager.data(), static_cast<void (MotionManager::*)(MotionStopCode, QString)>(&MotionManager::axisYMotionStopSignal));
            connect(motionInspector.data(), &AbstractMotionInspector::axisYForwardLimitSignal, motionManager.data(), &MotionManager::axisYForwardLimitSignal);
            connect(motionInspector.data(), &AbstractMotionInspector::axisYBackwardLimitSignal, motionManager.data(), &MotionManager::axisYBackwardLimitSignal);
            connect(motionInspector.data(), &AbstractMotionInspector::axisYHomeInProgressStartSignal, motionManager.data(), &MotionManager::axisYHomeInProgressStartSignal);
            connect(motionInspector.data(), &AbstractMotionInspector::axisYHomeInProgressStopSignal, motionManager.data(), &MotionManager::axisYHomeInProgressStopSignal);

            connect(motionInspector.data(), &AbstractMotionInspector::axisZMotorOffSignal, motionManager.data(), &MotionManager::axisZMotorOffSignal);
            connect(motionInspector.data(), static_cast<void (AbstractMotionInspector::*)()>(&AbstractMotionInspector::axisZMotionStopSignal),
                    motionManager.data(), static_cast<void (MotionManager::*)()>(&MotionManager::axisZMotionStopSignal));
            connect(motionInspector.data(), static_cast<void (AbstractMotionInspector::*)(MotionStopCode, QString)>(&AbstractMotionInspector::axisZMotionStopSignal),
                    motionManager.data(), static_cast<void (MotionManager::*)(MotionStopCode, QString)>(&MotionManager::axisZMotionStopSignal));
            connect(motionInspector.data(), &AbstractMotionInspector::axisZForwardLimitSignal, motionManager.data(), &MotionManager::axisZForwardLimitSignal);
            connect(motionInspector.data(), &AbstractMotionInspector::axisZBackwardLimitSignal, motionManager.data(), &MotionManager::axisZBackwardLimitSignal);
            connect(motionInspector.data(), &AbstractMotionInspector::axisZHomeInProgressStartSignal, motionManager.data(), &MotionManager::axisZHomeInProgressStartSignal);
            connect(motionInspector.data(), &AbstractMotionInspector::axisZHomeInProgressStopSignal, motionManager.data(), &MotionManager::axisZHomeInProgressStopSignal);

        }

    }

    // inizializzo il cn watcher per controllare la connessione
    initCNConnectionWatcher();

    // gestisco il PLC Galil
    if (s.getMachinePLCType() == DeviceKey::GALIL_PLC) {

        if (plc.isNull()) {

            plc.reset(new GalilPLCController());
            GalilPLCController::Ptr plcPtr = static_cast<GalilPLCController::Ptr>(plc.data());
            plcPtr->setupController(s.getGalilPLCNumberDigitalInput(), s.getGalilPLCNumberDigitalOutput(), s.getGalilPLCNumberAnalogInput());

        }

        // e lo aggiungo all'io manager
        traceDebug() << "Inserisco il PLC Galil ai dispositivi di IO";
        ioManager->addDevice(DeviceKey::GALIL_PLC, plc.staticCast<GalilPLCController>());

        initPLCConnectionWatcher();

    }

    traceExit;

}

void MainWindow::initMotionInspector() {

    traceEnter;

    Settings& s = Settings::instance();
    QThread* motionInspectorThread = new QThread();

    // qui ci va la lista dei cn.
    if (s.getMachineCNType() == DeviceKey::GALIL_CN)
        motionInspector.reset(new GalilCNInspector());

    if (motionInspector.isNull())
        return;

    connect(motionInspectorThread, &QThread::started, motionInspector.data(), &AbstractMotionInspector::startProcess);
    connect(motionInspector.data(), &AbstractMotionInspector::processStoppedSignal, motionInspectorThread, &QThread::quit);

    connect(motionInspector.data(), &AbstractMotionInspector::statusSignal, this, &MainWindow::motionStatusUpdateSignal);

    connect(motionInspectorThread, &QThread::finished, motionInspector.data(), &AbstractMotionInspector::deleteLater);
    connect(motionInspectorThread, &QThread::finished, motionInspectorThread, &QThread::deleteLater);

    motionInspector.data()->moveToThread(motionInspectorThread);

//    errorManager->subscribeObject(*motionInspector);

    traceExit;

}

void MainWindow::initGalilPLCInspector() {

    traceEnter;

    Settings& s = Settings::instance();

    if (s.getMachinePLCType() == DeviceKey::GALIL_PLC) {

        galilPLCInspector.reset(new GalilPLCInspector());

        QThread* galilPLCInspectorThread = new QThread();

        connect(galilPLCInspectorThread, &QThread::started, galilPLCInspector.data(), &AbstractConnectedDeviceInspector::startProcess);
        connect(galilPLCInspector.data(), &AbstractConnectedDeviceInspector::processStoppedSignal, galilPLCInspectorThread, &QThread::quit);

        connect(galilPLCInspector.data(), &AbstractConnectedDeviceInspector::statusSignal, this, &MainWindow::galilPLCStatusUpdateSignal);

        connect(galilPLCInspectorThread, &QThread::finished, galilPLCInspector.data(), &AbstractConnectedDeviceInspector::deleteLater);
        connect(galilPLCInspectorThread, &QThread::finished, galilPLCInspectorThread, &QThread::deleteLater);

        galilPLCInspector.data()->moveToThread(galilPLCInspectorThread);

//        errorManager->subscribeObject(*galilPLCInspector);
    }

    traceExit;

}

void MainWindow::initIOInspector() {

    traceEnter;

    QThread* ioInspectorThread = new QThread();

    ioInspector.reset(new IOInspector());

    connect(ioInspectorThread, &QThread::started, ioInspector.data(), &IOInspector::startProcess);
    connect(ioInspector.data(), &IOInspector::processStopSignal, ioInspectorThread, &QThread::quit);

    connect(this, &MainWindow::motionStatusUpdateSignal, [&](const QVariant& status) {
        QMetaObject::invokeMethod(ioInspector.data(), "updateIOStatus", Qt::QueuedConnection,
                                  Q_ARG(DeviceKey, DeviceKey::GALIL_CN),
                                  Q_ARG(const QVariant&, status));
    });
    connect(this, &MainWindow::galilPLCStatusUpdateSignal, [&](const QVariant& status) {
        QMetaObject::invokeMethod(ioInspector.data(), "updateIOStatus", Qt::QueuedConnection,
                                  Q_ARG(DeviceKey, DeviceKey::GALIL_PLC),
                                  Q_ARG(const QVariant&, status));
    });
    connect(ioInspector.data(), &IOInspector::statusSignal, this, &MainWindow::ioStatusUpdateSignal);

    connect(ioInspectorThread, &QThread::finished, ioInspector.data(), &IOInspector::deleteLater);
    connect(ioInspectorThread, &QThread::finished, ioInspectorThread, &QThread::deleteLater);

    ioInspector.data()->moveToThread(ioInspectorThread);

    traceExit;

}

void MainWindow::initCNConnectionWatcher() {

    traceEnter;

    if (cn.isNull()) {
        // TODO NIC 07/12/2018 - lanciare eccezione se non e' impostato alcun cn
        return;
    }

    Settings& s = Settings::instance();
    if (s.getMachineCNType() == DeviceKey::GALIL_CN) {
        cnConnectionWatcher.reset(new GalilCNConnectionWatcher());
        cnConnectionWatcher->setDevice<DeviceKey::GALIL_CN>(cn.staticCast<GalilCNController>());
    }

    if (cnConnectionWatcher.isNull())
        return;

    // NOTE NIC 21/12/2018: non creo un thread a parte perche' il device e' associato al thread main;
    // usare il device in un altro thread potrebbe creare problemi

    /*****************************************************************
    QThread* watcherThread = new QThread();

    connect(watcherThread, &QThread::started, cnConnectionWatcher.data(), &AbstractDeviceConnectionWatcher::startWatcher);
    connect(cnConnectionWatcher.data(), &AbstractDeviceConnectionWatcher::watcherStopped, watcherThread, &QThread::quit);

    connect(watcherThread, &QThread::finished, cnConnectionWatcher.data(), &AbstractDeviceConnectionWatcher::deleteLater);
    connect(watcherThread, &QThread::finished, watcherThread, &QThread::deleteLater);

    cnConnectionWatcher->moveToThread(watcherThread);
    *****************************************************************/

    traceExit;

}

void MainWindow::initPLCConnectionWatcher() {

    traceEnter;

    if (plc.isNull()) {
        // TODO NIC 07/12/2018 - lanciare eccezione se non e' impostato alcun plc
        return;
    }

    Settings& s = Settings::instance();
    if (s.getMachinePLCType() == DeviceKey::GALIL_PLC) {
        plcConnectionWatcher.reset(new GalilPLCConnectionWatcher());
        plcConnectionWatcher->setDevice<DeviceKey::GALIL_PLC>(plc.staticCast<GalilPLCController>());
    }

    if (plcConnectionWatcher.isNull())
        return;

    // NOTE NIC 21/12/2018: non creo un thread a parte perche' il device e' associato al thread main;
    // usare il device in un altro thread potrebbe creare problemi

    /*****************************************************************
    QThread* watcherThread = new QThread();

    connect(watcherThread, &QThread::started, plcConnectionWatcher.data(), &AbstractDeviceConnectionWatcher::startWatcher);
    connect(plcConnectionWatcher.data(), &AbstractDeviceConnectionWatcher::watcherStopped, watcherThread, &QThread::quit);

    connect(watcherThread, &QThread::finished, plcConnectionWatcher.data(), &AbstractDeviceConnectionWatcher::deleteLater);
    connect(watcherThread, &QThread::finished, watcherThread, &QThread::deleteLater);

    plcConnectionWatcher->moveToThread(watcherThread);
    *****************************************************************/

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

    if (!motionInspector.isNull())
        motionInspector->thread()->start();

    if (!cnConnectionWatcher.isNull())
        cnConnectionWatcher->startWatcher();

    if (!galilPLCInspector.isNull())
        galilPLCInspector->thread()->start();

    if (!plcConnectionWatcher.isNull())
        plcConnectionWatcher->startWatcher();

    if (!ioInspector.isNull())
        ioInspector->thread()->start();

    traceExit;

}

void MainWindow::stopDevices() {

    traceEnter;

    if (!motionInspector.isNull())
        motionInspector->stopProcess();

    if (!cnConnectionWatcher.isNull())
        cnConnectionWatcher->stopWatcher();

    if (!galilPLCInspector.isNull())
        galilPLCInspector->stopProcess();

    if (!plcConnectionWatcher.isNull())
        plcConnectionWatcher->stopWatcher();

    if (!ioInspector.isNull())
        ioInspector->stopProcess();

    traceExit;

}


