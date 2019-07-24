#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QMetaObject>

#include <QFile>
#include <Logger.hpp>
#include <gui/resources/lang/lang.hpp>

#include <DeviceFactory.hpp>


static constexpr int MAINWINDOW_ITEM_ALERT_POSITION = 0;

static constexpr char MAINWINDOW_MDCUSTOMITEM_ALERT_OBJECT_NAME[] = "alertItem";
static constexpr char MAINWINDOW_MDCUSTOMITEM_IO_OBJECT_NAME[] = "ioItem";
static constexpr char MAINWINDOW_MDCUSTOMITEM_MOTION_OBJECT_NAME[] = "motionItem";
static constexpr char MAINWINDOW_MDCUSTOMITEM_FORI_OBJECT_NAME[] = "foriItem";
static constexpr char MAINWINDOW_MDCUSTOMITEM_FILE_EXPLORER_OBJECT_NAME[] = "fileExplorerItem";

static constexpr char MAINWINDOW_PB_ERROR_STATE_ERROR_TYPE_PROPERTY[] = "ErrorType";
static constexpr char MAINWINDOW_PB_ERROR_STATE_ERROR_TYPE_PROPERTY_FATAL[] = "FATAL";
static constexpr char MAINWINDOW_PB_ERROR_STATE_ERROR_TYPE_PROPERTY_ERROR[] = "ERROR";
static constexpr char MAINWINDOW_PB_ERROR_STATE_ERROR_TYPE_PROPERTY_WARNING[] = "WARNING";
static constexpr char MAINWINDOW_PB_ERROR_STATE_ERROR_TYPE_PROPERTY_INFO[] = "INFO";


using namespace PROGRAM_NAMESPACE;

MainWindow::MainWindow(QWidget *parent) :
    UnmovableWindow(parent), ui(new Ui::MainWindow) {

    traceEnter;

//    msn = DeviceFactoryInstance.instanceMachineStatusNotifier();
    this->setupUi();
    this->setupStyleSheets();

    this->setupSignalsAndSlots();

    traceExit;

}

MainWindow::~MainWindow() {

    traceEnter;

    DeviceFactoryInstance.stop();
    DeviceFactoryInstance.detachManagers();
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

    // left panel
    auto&& listItem = ui->listItem;
    connect(listItem, &MDListWidget::itemClicked, [&, listItem](QListWidgetItem* item) {
        auto itemWidget = static_cast<MDCustomItem::Ptr>(listItem->itemWidget(item));
        if (itemWidget == nullptr)
            return;
        QString text = itemWidget->text();
        if (QString(MAINWINDOW_MDCUSTOMITEM_ALERT).compare(text) == 0)
            ui->stackedWidget->setCurrentWidget(ui->pageAlert);
        else if (QString(MAINWINDOW_MDCUSTOMITEM_MOTION).compare(text) == 0)
            ui->stackedWidget->setCurrentWidget(ui->pageMotion);
        else if (QString(MAINWINDOW_MDCUSTOMITEM_IO).compare(text) == 0)
            ui->stackedWidget->setCurrentWidget(ui->pageIO);
        else if (QString(MAINWINDOW_MDCUSTOMITEM_FILE_EXPLORER).compare(text) == 0)
            ui->stackedWidget->setCurrentWidget(ui->pageFileExplorer);

    });

    // top panel
    QWeakPointer<ErrorManager> errorManager = DeviceFactoryInstance.getErrorManager();
    connect(errorManager.data(), &ErrorManager::notifyMaxErrorType, this, [&](const ErrorType& errType) {
        switch (errType) {
        case ErrorType::INFO: ui->pbErrorState->setProperty(MAINWINDOW_PB_ERROR_STATE_ERROR_TYPE_PROPERTY, MAINWINDOW_PB_ERROR_STATE_ERROR_TYPE_PROPERTY_INFO); break;
        case ErrorType::WARNING: ui->pbErrorState->setProperty(MAINWINDOW_PB_ERROR_STATE_ERROR_TYPE_PROPERTY, MAINWINDOW_PB_ERROR_STATE_ERROR_TYPE_PROPERTY_WARNING); break;
        case ErrorType::ERROR: ui->pbErrorState->setProperty(MAINWINDOW_PB_ERROR_STATE_ERROR_TYPE_PROPERTY, MAINWINDOW_PB_ERROR_STATE_ERROR_TYPE_PROPERTY_ERROR); break;
        case ErrorType::FATAL: ui->pbErrorState->setProperty(MAINWINDOW_PB_ERROR_STATE_ERROR_TYPE_PROPERTY, MAINWINDOW_PB_ERROR_STATE_ERROR_TYPE_PROPERTY_FATAL); break;
        }
        ui->pbErrorState->style()->unpolish(ui->pbErrorState);
        ui->pbErrorState->style()->polish(ui->pbErrorState);
    }, Qt::AutoConnection);

    connect(ui->pbErrorState, &QPushButton::clicked, [&]() {
        ui->listItem->setCurrentRow(MAINWINDOW_ITEM_ALERT_POSITION);
        ui->stackedWidget->setCurrentWidget(ui->pageAlert);
    });

    traceExit;

}

void MainWindow::setupUiPanels() {

    traceEnter;

    setupUiTopPanel();
    setupUiLeftPanel();
    setupUiContentPanel();

    traceExit;

}

void MainWindow::setupUiTopPanel() {

    traceEnter;
    // nothing to do
    traceExit;

}

void MainWindow::setupUiLeftPanel() {

    traceEnter;

    MDCustomItem::Ptr alert = new MDCustomItem;
    alert->setObjectName(MAINWINDOW_MDCUSTOMITEM_ALERT_OBJECT_NAME);
    alert->setText(MAINWINDOW_MDCUSTOMITEM_ALERT);

    MDCustomItem::Ptr io = new MDCustomItem;
    io->setObjectName(MAINWINDOW_MDCUSTOMITEM_IO_OBJECT_NAME);
    io->setText(MAINWINDOW_MDCUSTOMITEM_IO);

    MDCustomItem::Ptr motion = new MDCustomItem;
    motion->setObjectName(MAINWINDOW_MDCUSTOMITEM_MOTION_OBJECT_NAME);
    motion->setText(MAINWINDOW_MDCUSTOMITEM_MOTION);

    MDCustomItem::Ptr fileExplorer = new MDCustomItem;
    fileExplorer->setObjectName(MAINWINDOW_MDCUSTOMITEM_FILE_EXPLORER_OBJECT_NAME);
    fileExplorer->setText(MAINWINDOW_MDCUSTOMITEM_FILE_EXPLORER);


    QListWidgetItem* alertItem = new QListWidgetItem(ui->listItem);
    QListWidgetItem* ioItem = new QListWidgetItem(ui->listItem);
    QListWidgetItem* motionItem = new QListWidgetItem(ui->listItem);
    QListWidgetItem* fileExplorerItem = new QListWidgetItem(ui->listItem);

    ui->listItem->addItem(alertItem);
    ui->listItem->addItem(ioItem);
    ui->listItem->addItem(motionItem);
    ui->listItem->addItem(fileExplorerItem);
    ui->listItem->setItemWidget(alertItem, alert);
    ui->listItem->setItemWidget(ioItem, io);
    ui->listItem->setItemWidget(motionItem, motion);
    ui->listItem->setItemWidget(fileExplorerItem, fileExplorer);

    traceExit;

}

void MainWindow::setupUiContentPanel() {

    traceEnter;

    traceExit;

}

/*

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

    // NOTE NIC 21/12/2018 - non creo un thread a parte perche' il device e' associato al thread main;
    // usare il device in un altro thread potrebbe creare problemi

    //QThread* watcherThread = new QThread();
    //
    //connect(watcherThread, &QThread::started, cnConnectionWatcher.data(), &AbstractDeviceConnectionWatcher::startWatcher);
    //connect(cnConnectionWatcher.data(), &AbstractDeviceConnectionWatcher::watcherStopped, watcherThread, &QThread::quit);
    //
    //connect(watcherThread, &QThread::finished, cnConnectionWatcher.data(), &AbstractDeviceConnectionWatcher::deleteLater);
    //connect(watcherThread, &QThread::finished, watcherThread, &QThread::deleteLater);
    //
    //cnConnectionWatcher->moveToThread(watcherThread);

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

    // NOTE NIC 21/12/2018 - non creo un thread a parte perche' il device e' associato al thread main;
    // usare il device in un altro thread potrebbe creare problemi

    // QThread* watcherThread = new QThread();
    //
    // connect(watcherThread, &QThread::started, plcConnectionWatcher.data(), &AbstractDeviceConnectionWatcher::startWatcher);
    // connect(plcConnectionWatcher.data(), &AbstractDeviceConnectionWatcher::watcherStopped, watcherThread, &QThread::quit);
    //
    // connect(watcherThread, &QThread::finished, plcConnectionWatcher.data(), &AbstractDeviceConnectionWatcher::deleteLater);
    // connect(watcherThread, &QThread::finished, watcherThread, &QThread::deleteLater);
    //
    // plcConnectionWatcher->moveToThread(watcherThread);

    traceExit;

}

*/

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

//    if (msn->getCurrentStatus() == MachineStatus::IDLE)
//        msn->setCurrentStatus(MachineStatus::PRINTING);
//    else
//        msn->setCurrentStatus(MachineStatus::IDLE);

    traceExit;

}
