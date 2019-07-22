#include "AbstractConnectedDeviceInspector.hpp"

using namespace PROGRAM_NAMESPACE;

AbstractConnectedDeviceInspector::AbstractConnectedDeviceInspector(QObject* parent) :
    AbstractDeviceInspector(parent) {

    connect(this, &AbstractConnectedDeviceInspector::disconnectedSignal, this, &AbstractDeviceInspector::restartProcess);

}

bool AbstractConnectedDeviceInspector::beforeProcess() {

    traceEnter;
    if (!connectDevice()) {
        QTimer::singleShot(restartTimeMs, this, &AbstractConnectedDeviceInspector::startProcess);
        emit failedToConnectSignal();
        return false;
    } else
        emit connectedSignal();
    traceExit;
    return true;

}

void AbstractConnectedDeviceInspector::process() {

    traceEnter;
    if (isDeviceConnected()) {
        AbstractDeviceInspector::process();
    } else
        manageDisconnectedDevice();
    traceExit;

}

void AbstractConnectedDeviceInspector::manageDisconnectedDevice() {

    traceEnter;
    if (refreshTimer.isActive())
        refreshTimer.stop();
    this->handleDisconnection();
    emit disconnectedSignal();
    traceExit;

}
