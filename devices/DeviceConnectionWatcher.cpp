#include "DeviceConnectionWatcher.hpp"

#include <Logger.hpp>
#include <common/Utils.hpp>

using namespace PROGRAM_NAMESPACE;

DeviceConnectionWatcher::DeviceConnectionWatcher(QObject* parent) :
    QObject(parent), device(nullptr),
    connectionCheckTimer(this), isConnected(false) {

    // TODO NIC 30/05/2019 - capire in futuro se abilitarlo o meno
    // if (QThread::currentThread()->eventDispatcher()==nullptr)
    //     throw ThreadNoEventLoopException();

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
            emit deviceConnected();
            isConnected = true;
        }

    } else {

        if (isConnected == true)
            isConnected = false;

        emit deviceDisconnected();

        reconnectDevice();

    }

    traceExit;

}
