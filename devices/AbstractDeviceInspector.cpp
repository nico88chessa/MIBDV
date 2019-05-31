#include "AbstractDeviceInspector.hpp"

using namespace PROGRAM_NAMESPACE;

AbstractDeviceInspector::AbstractDeviceInspector(QObject *parent) :
    QObject(parent), refreshTimer(this) {

    refreshTimer.setInterval(DEFAULT_INTERVAL_MS);
    restartTimeMs = DEFAULT_RESTART_DELAY_TIME_MS;

    initializeInspector();

    connect(&refreshTimer, &QTimer::timeout, this, &AbstractDeviceInspector::process);

}

AbstractDeviceInspector::~AbstractDeviceInspector() {

    traceEnter;
    stopProcess();
    traceExit;

}

void AbstractDeviceInspector::setRefreshValue(int intervalMs) {

    traceEnter;
    refreshTimer.setInterval(intervalMs);
    traceExit;

}

void AbstractDeviceInspector::setRestartTime(int restartTimeMs) {

    traceEnter;
    this->restartTimeMs = restartTimeMs;
    traceExit;

}

void AbstractDeviceInspector::initializeInspector() {
    // do nothing
}

bool AbstractDeviceInspector::beforeProcess() {
    return true;
}

void AbstractDeviceInspector::afterStop() {
    // do nothing
}

void AbstractDeviceInspector::process() {

    traceEnter;
    QVariant status;
    if (getStatus(status))
        emit statusSignal(status);
    emit isRunningSignal(true);
    traceExit;

}

void AbstractDeviceInspector::startProcess() {

    traceEnter;
    if (!this->beforeProcess()) {
        emit isRunningSignal(false);
        return;
    }
    refreshTimer.start();
    emit processStartedSignal();
    traceExit;

}

void AbstractDeviceInspector::stopProcess() {

    traceEnter;
    if (refreshTimer.isActive()) {
        refreshTimer.stop();
        emit processStoppedSignal();
    }
    afterStop();
    traceExit;

}

void AbstractDeviceInspector::restartProcess() {

    traceEnter;
    if (refreshTimer.isActive())
        refreshTimer.stop();
    QTimer::singleShot(restartTimeMs, this, &AbstractDeviceInspector::startProcess);
    traceExit;

}
