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
    galilCNInspector->thread()->wait();
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

    if (ioManager.isNull())
        ioManager.reset(new IOManager());

    // gestisco il CN Galil
    if (s.getMachineCNType() == DeviceKey::GALIL_CN) {

        // per prima cosa, avvio l'inspector del CN galil
        this->initGalilCNInspector();

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

            // controllo che gli oggetti siano validi
            if (!(galilCNInspector.isNull() || motionManager.isNull())) {
                // TODO NIC 03/12/2018 - completare gli altri segnali
                connect(galilCNInspector.data(), &GalilCNInspector::powerOffSignal, motionManager.data(), &MotionManager::powerOffSignal);
                connect(galilCNInspector.data(), &GalilCNInspector::cycleOffSignal, motionManager.data(), &MotionManager::cycleOffSignal);
                connect(galilCNInspector.data(), &GalilCNInspector::axisXMotorOffSignal, motionManager.data(), &MotionManager::axisXMotorOffSignal);
                connect(galilCNInspector.data(), &GalilCNInspector::axisXMotionStopSignal, motionManager.data(), &MotionManager::axisXMotionStopSignal);
                connect(galilCNInspector.data(), &GalilCNInspector::axisXForwardLimitSignal, motionManager.data(), &MotionManager::axisXForwardLimitSignal);
                connect(galilCNInspector.data(), &GalilCNInspector::axisXBackwardLimitSignal, motionManager.data(), &MotionManager::axisXBackwardLimitSignal);
            }

        }

        // infine inizializzo il cn watcher per controllare la connessione
        initCNConnectionWatcher();

    }

    // gestisco il PLC Galil
    if (s.getMachinePLCType() == DeviceKey::GALIL_PLC) {

        // per prima cosa, avvio l'inspector del PLC galil
        this->initGalilPLCInspector();

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

    // TODO NIC 06/12/2018 - da parametrizzare in file di configurazione timers
//    cnConnectionWatcher->setupTimers(1000);
//    connect(cnConnectionWatcher.data(), &DeviceConnectionWatcher::deviceDisconnected, this, &MainWindow::handleDisconnectionCN);
//    cnConnectionWatcher->startWatcher();

//    // TODO NIC 06/12/2018 - da parametrizzare in file di configurazione timers
//    plcConnectionWatcher->setupTimers(1000);
//    connect(plcConnectionWatcher.data(), &DeviceConnectionWatcher::deviceDisconnected, this, &MainWindow::handleDisconnectionPLC);
//    plcConnectionWatcher->startWatcher();

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

void MainWindow::initCNConnectionWatcher() {

    traceEnter;

    if (cn.isNull()) {
        // TODO NIC 07/12/2018 - lanciare eccezione se non e' impostato alcun cn
        return;
    }

    class GalilCNDeviceConnetionWatcher : public DeviceConnectionWatcher {
    private:
        QTimer timerKeepAlive;
    public:
        explicit GalilCNDeviceConnetionWatcher(QObject* parent = nullptr) :
            DeviceConnectionWatcher(parent),
            timerKeepAlive(this) {

            int intervalMs = Settings::instance().getGalilCNCheckConnectionIntervalMs();
            this->setupTimers(intervalMs);

            connect(&timerKeepAlive, &QTimer::timeout, [this]() {
                traceEnter;
                unsigned int timeMs;
                device.staticCast<GalilCNController>()->getKeepAliveTimeMs(&timeMs);
                traceExit;
            });

        }

        ~GalilCNDeviceConnetionWatcher() {
            timerKeepAlive.stop();
        }

    protected:
        virtual void reconnectDevice() {

            traceEnter;
            if (!device.isNull()) {

                if (!device->isConnected()) {

                    timerKeepAlive.stop();

                    QString ipCN = Settings::instance().getGalilCNIpAddress();
                    auto galilCN = device.staticCast<GalilCNController>().data();

                    if (galilCN->connect(ipCN)) {
                        unsigned int timeMs;
                        int res = galilCN->getKeepAliveTimeMs(&timeMs);
                        if (galilCN->isError(res)) {

                            traceErr() << "Errore chiamata CN keep alive; codice errore:" << res;
                            traceErr() << "Descrizione errore:" << galilCN->decodeError(res);

                        } else {

                            timerKeepAlive.setInterval(timeMs / 2);
                            timerKeepAlive.start();
                        }
                    }
                }
            }

            traceExit;
            return;
        }
    };

    QThread* cnWatcherThread = new QThread();

    cnConnectionWatcher.reset(new GalilCNDeviceConnetionWatcher());
    cnConnectionWatcher->setDevice<DeviceKey::GALIL_CN>(cn.staticCast<GalilCNController>());

    connect(cnWatcherThread, &QThread::started, cnConnectionWatcher.data(), &GalilCNDeviceConnetionWatcher::startWatcher);
    connect(cnConnectionWatcher.data(), &GalilCNDeviceConnetionWatcher::watcherStopped, cnWatcherThread, &QThread::quit);

    connect(cnWatcherThread, &QThread::finished, cnConnectionWatcher.data(), &GalilCNDeviceConnetionWatcher::deleteLater);
    connect(cnWatcherThread, &QThread::finished, cnWatcherThread, &QThread::deleteLater);

    cnConnectionWatcher->moveToThread(cnWatcherThread);

    traceExit;

}

void MainWindow::initPLCConnectionWatcher() {

    traceEnter;

    if (plc.isNull()) {
        // TODO NIC 07/12/2018 - lanciare eccezione se non e' impostato alcun plc
        return;
    }

    class GalilPLCDeviceConnetionWatcher : public DeviceConnectionWatcher {
    private:
        QTimer timerKeepAlive;
    public:
        explicit GalilPLCDeviceConnetionWatcher(QObject* parent = nullptr) :
            DeviceConnectionWatcher(parent),
            timerKeepAlive(this) {

            int intervalMs = Settings::instance().getGalilPLCCheckConnectionIntervalMs();
            this->setupTimers(intervalMs);

            connect(&timerKeepAlive, &QTimer::timeout, [this]() {
                traceEnter;
                unsigned int timeMs;
                device.staticCast<GalilPLCController>()->getKeepAliveTimeMs(&timeMs);
                traceExit;
            });

        }

        ~GalilPLCDeviceConnetionWatcher() {
            timerKeepAlive.stop();
        }

    protected:
        virtual void reconnectDevice() {

            traceEnter;
            if (!device.isNull()) {

                if (!device->isConnected()) {

                    timerKeepAlive.stop();

                    QString ipPLC = Settings::instance().getGalilPLCIpAddress();
                    auto galilPLC = device.staticCast<GalilPLCController>().data();

                    if (galilPLC->connect(ipPLC)) {
                        unsigned int timeMs;
                        int res = galilPLC->getKeepAliveTimeMs(&timeMs);
                        if (galilPLC->isError(res)) {

                            traceErr() << "Errore chiamata PLC keep alive; codice errore:" << res;
                            traceErr() << "Descrizione errore:" << galilPLC->decodeError(res);

                        } else {

                            timerKeepAlive.setInterval(timeMs / 2);
                            timerKeepAlive.start();
                        }
                    }
                }
            }

            traceExit;
            return;
        }
    };

    QThread* cnWatcherThread = new QThread();

    plcConnectionWatcher.reset(new GalilPLCDeviceConnetionWatcher());
    plcConnectionWatcher->setDevice<DeviceKey::GALIL_PLC>(plc.staticCast<GalilPLCController>());

    connect(cnWatcherThread, &QThread::started, plcConnectionWatcher.data(), &GalilPLCDeviceConnetionWatcher::startWatcher);
    connect(plcConnectionWatcher.data(), &GalilPLCDeviceConnetionWatcher::watcherStopped, cnWatcherThread, &QThread::quit);

    connect(cnWatcherThread, &QThread::finished, plcConnectionWatcher.data(), &GalilPLCDeviceConnetionWatcher::deleteLater);
    connect(cnWatcherThread, &QThread::finished, cnWatcherThread, &QThread::deleteLater);

    plcConnectionWatcher->moveToThread(cnWatcherThread);

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
        if (!galilCNInspector.isNull()) {
            QThread* galilCNThread = galilCNInspector->thread();
            galilCNThread->start();
        }

        if (!cnConnectionWatcher.isNull())
            cnConnectionWatcher->thread()->start();

    }

    if (s.getMachinePLCType() == DeviceKey::GALIL_PLC) {
        if (!galilPLCInspector.isNull()) {
            QThread* galilPLCThread = galilPLCInspector->thread();
            galilPLCThread->start();
        }

        if (!plcConnectionWatcher.isNull())
            plcConnectionWatcher->thread()->start();

    }

    traceExit;

}

void MainWindow::stopDevices() {

    traceEnter;

    Settings& s = Settings::instance();

    if (s.getMachineCNType() == DeviceKey::GALIL_CN)
        if (!galilCNInspector.isNull())
            galilCNInspector->stopProcess();

    if (s.getMachinePLCType() == DeviceKey::GALIL_PLC)
        if (!galilPLCInspector.isNull())
            galilPLCInspector->stopProcess();

    if (!cnConnectionWatcher.isNull())
        cnConnectionWatcher->stopWatcher();

    if (!plcConnectionWatcher.isNull())
        plcConnectionWatcher->stopWatcher();

    traceExit;

}


