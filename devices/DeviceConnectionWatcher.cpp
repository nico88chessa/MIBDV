#include "DeviceConnectionWatcher.hpp"

#include <Logger.hpp>
#include <common/Utils.hpp>


using namespace PROGRAM_NAMESPACE;

static const ErrorID DEVICE_CONNECTION_WATCHER_DISCONNECTED = PROGRAM_ERR_START_CODE + 1;

static constexpr char DEVICE_CONNECTION_WATCHER_DISCONNECTED_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Connection watcher: device is not connected.");

DeviceConnectionWatcher::DeviceConnectionWatcher(int watcherId, QObject* parent) :
    QObject(parent), device(nullptr),
    connectionCheckTimer(this), isConnected(false), instanceId(watcherId) {

    // TODO NIC 30/05/2019 - capire in futuro se abilitarlo o meno
    // if (QThread::currentThread()->eventDispatcher()==nullptr)
    //     throw ThreadNoEventLoopException();

    connect(&connectionCheckTimer, &QTimer::timeout, this, &DeviceConnectionWatcher::checkConnection);
    initializeErrorSignaler(errorSignaler, this);

}

DeviceConnectionWatcher::~DeviceConnectionWatcher() {

    traceEnter;
    connectionCheckTimer.stop();
    traceExit;

}

void DeviceConnectionWatcher::setupTimers(int checkIntervalMs) {

    traceEnter;
    connectionCheckTimer.setInterval(checkIntervalMs);
    traceExit;

}

void DeviceConnectionWatcher::reconnectDevice() {

    traceEnter;

    if (!device.isNull() && !device.data()->isConnected())
        device.data()->connect();

    traceExit;

}

void DeviceConnectionWatcher::startWatcher() {

    traceEnter;
    connectionCheckTimer.start();
    emit watcherStarted();
    traceExit;

}

void DeviceConnectionWatcher::stopWatcher() {

    traceEnter;
    connectionCheckTimer.stop();
    emit watcherStopped();
    traceExit;

}

void DeviceConnectionWatcher::checkConnection() {

    traceEnter;

    if (device.data()->isConnected()) {

        if (isConnected == false) {
            errorSignaler->clearErrors();
            errorSignaler->notifyErrors();
            emit deviceConnected();
            isConnected = true;
        }

    } else {

        if (isConnected == true)
            isConnected = false;

        emit deviceDisconnected();
        errorSignaler->clearErrors();
        errorSignaler->addError(Error(DeviceKey::DEVICE_CONNECTION_WATCHER,
                                      DEVICE_CONNECTION_WATCHER_DISCONNECTED,
                                      QString("%1 Device: %2").arg(DEVICE_CONNECTION_WATCHER_DISCONNECTED_DESCR).arg(Utils::getStringFromDeviceKey(deviceKey)),
                                      ErrorType::FATAL,
                                      this->instanceId));
        errorSignaler->notifyErrors();

        reconnectDevice();

    }

    traceExit;

}
