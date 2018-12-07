#include "GalilPLCInspector.hpp"

#include <Settings.hpp>
#include <Logger.hpp>

using namespace PROGRAM_NAMESPACE;

GalilPLCInspector::GalilPLCInspector(QObject* parent) :
    QObject(parent),
    controller(new GalilPLCController()),
    refreshTimer(this),
    errorSignaler(new ErrorSignaler(this)) {

    Settings& s = Settings::instance();

    ipAddress = s.getGalilPLCIpAddress();
    reconnectionIntervalMs = s.getGalilPLCReconnectionIntervalMs();
    int digitalInput = s.getGalilPLCNumberDigitalInput();
    int digitalOutput = s.getGalilPLCNumberDigitalOutput();
    int analogInput = s.getGalilPLCNumberAnalogInput();

    controller->setupController(digitalInput, digitalOutput, analogInput);

    int refreshTimeMs = s.getGalilPLCStatusRefreshIntervalMs();
    refreshTimer.setInterval(refreshTimeMs);

    connect(&refreshTimer, &QTimer::timeout, this, &GalilPLCInspector::process);
    connect(this, &GalilPLCInspector::disconnectedSignal, this, &GalilPLCInspector::restartProcess);

}

GalilPLCInspector::~GalilPLCInspector() {

    traceEnter;

    // errorSignaler viene eliminato automaticamente
    refreshTimer.stop();

    traceExit;
}

void GalilPLCInspector::process() {

    traceEnter;

    if (controller->isConnected()) {

        try {
            auto status = controller->getStatus();
            emit statusSignal(status);

        } catch (NoStatusException& e) {
            Q_UNUSED(e)
            traceErr() << "Errore nel recupero dello stato da GalilPLCInspector";
        }

    } else
        handleDisconnection();

    traceExit;

}

void GalilPLCInspector::handleDisconnection() {

    traceEnter;

    refreshTimer.stop();

    errorSignaler->addError(Error(static_cast<int>(DeviceKey::GALIL_CN), G_TIMEOUT, GalilControllerUtils::getErrorDescription(G_TIMEOUT), ErrorType::ERROR));

    emit disconnectedSignal();

    traceExit;

}

void GalilPLCInspector::restartProcess() {

    traceEnter;

    if (refreshTimer.isActive())
        refreshTimer.stop();

    QTimer::singleShot(reconnectionIntervalMs, this, &GalilPLCInspector::startProcess);

    traceExit;

}



void GalilPLCInspector::startProcess() {

    traceEnter;

    if (!controller->connect(ipAddress)) {
        traceErr() << "GalilPLCInspector: connessione al PLC fallita";
        QTimer::singleShot(reconnectionIntervalMs, this, &GalilPLCInspector::startProcess);
        return;
    } else
        emit connectedSignal();

    refreshTimer.start();

    emit processStartSignal();
    traceExit;

}

void GalilPLCInspector::stopProcess() {

    traceEnter;

    if (refreshTimer.isActive())
        refreshTimer.stop();

    emit processStopSignal();
    traceExit;

}
