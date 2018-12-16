#include "AbstractConnectedDeviceInspector.hpp"

using namespace PROGRAM_NAMESPACE;

AbstractConnectedDeviceInspector::AbstractConnectedDeviceInspector(QObject* parent) :
    AbstractDeviceInspector(parent) {

    connect(this, &AbstractConnectedDeviceInspector::disconnectedSignal, this, &AbstractConnectedDeviceInspector::restartProcess);

}

bool AbstractConnectedDeviceInspector::beforeProcess() {

    traceEnter;
    if (!connectDevice()) {
        QTimer::singleShot(restartTimeMs, this, &AbstractConnectedDeviceInspector::startProcess);
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
    this->stopProcess();
    this->handleDisconnection();
    emit disconnectedSignal();
    traceExit;

}
