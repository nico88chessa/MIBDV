#include "DeviceConnectionWatcher.hpp"

#include <Logger.hpp>
#include <common/Utils.hpp>

using namespace PROGRAM_NAMESPACE;

DeviceConnectionWatcher::DeviceConnectionWatcher(QObject* parent) :
    QObject(parent), device(nullptr),
    connectionCheckTimer(this), isConnected(false) {

    connect(&connectionCheckTimer, &QTimer::timeout, this, &DeviceConnectionWatcher::checkConnection);

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

    if (device->isConnected()) {

        if (isConnected == false) {
            emit deviceConnected();
            isConnected = true;
        }

    } else {

        if (isConnected == true)
            isConnected = false;

        traceErr() << "Il device" << Utils::getStringFromDeviceKey(key) << "non e' connesso";
        emit deviceDisconnected();

        reconnectDevice();

    }

    traceExit;

}
