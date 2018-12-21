#include "AbstractDeviceConnectionWatcher.hpp"

#include <Logger.hpp>
#include <common/Utils.hpp>

using namespace PROGRAM_NAMESPACE;

AbstractDeviceConnectionWatcher::AbstractDeviceConnectionWatcher(QObject* parent) :
    QObject(parent), device(nullptr),
    connectionCheckTimer(this), isConnected(false) {

    connect(&connectionCheckTimer, &QTimer::timeout, this, &AbstractDeviceConnectionWatcher::checkConnection);

}

AbstractDeviceConnectionWatcher::~AbstractDeviceConnectionWatcher() {

    traceEnter;
    connectionCheckTimer.stop();
    traceExit;

}

void AbstractDeviceConnectionWatcher::setupTimers(int checkIntervalMs) {

    traceEnter;
    connectionCheckTimer.setInterval(checkIntervalMs);
    traceExit;

}

void AbstractDeviceConnectionWatcher::startWatcher() {

    traceEnter;
    connectionCheckTimer.start();
    emit watcherStarted();
    traceExit;

}

void AbstractDeviceConnectionWatcher::stopWatcher() {

    traceEnter;
    connectionCheckTimer.stop();
    emit watcherStopped();
    traceExit;

}

void AbstractDeviceConnectionWatcher::checkConnection() {

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
