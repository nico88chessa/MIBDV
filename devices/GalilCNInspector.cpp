#include "GalilCNInspector.hpp"

using namespace PROGRAM_NAMESPACE;

GalilCNInspector::GalilCNInspector(QObject* parent) : QObject(parent),
    controller(new GalilCNController()),
    errorSignaler(new ErrorSignaler(this)) {

    Settings& s = Settings::instance();

    ipAddress = s.getGalilCNIpAddress();
    reconnectionIntervalMs = s.getGalilCNReconnectionIntervalMs();
    int digitalInput = s.getGalilCNNumberDigitalInput();
    int digitalOutput = s.getGalilCNNumberDigitalOutput();
    int analogInput = s.getGalilCNNumberAnalogInput();

    controller->setupController(digitalInput, digitalOutput, analogInput);

    int refreshTimeMs = s.getGalilCNStatusRefreshIntervalMs();
    refreshTimer.setInterval(refreshTimeMs);

    connect(&refreshTimer, &QTimer::timeout, this, &GalilCNInspector::process);
    connect(this, &GalilCNInspector::disconnectedSignal, this, &GalilCNInspector::restartProcess);

}

GalilCNInspector::~GalilCNInspector() {

    traceEnter;

    // errorSignaler viene eliminato automaticamente
    refreshTimer.stop();

    traceExit;
}

void GalilCNInspector::process() {

    traceEnter;

    if (controller->isConnected()) {

        auto status = controller->getStatus();
        emit statusSignal(status);

    } else
        handleDisconnection();


    traceExit;

}

void GalilCNInspector::handleDisconnection() {

    traceEnter;

    refreshTimer.stop();

    errorSignaler->addError(Error(static_cast<int>(DeviceKey::GALIL_CN), G_TIMEOUT, GalilControllerUtils::getErrorDescription(G_TIMEOUT), ErrorType::ERROR));

    emit disconnectedSignal();

    traceExit;

}

void GalilCNInspector::restartProcess() {

    traceEnter;

    if (refreshTimer.isActive())
        refreshTimer.stop();

    QTimer::singleShot(reconnectionIntervalMs, this, &GalilCNInspector::startProcess);

    traceExit;

}



void GalilCNInspector::startProcess() {

    traceEnter;

    // TODO NIC 26/10/2018 - parametrizzare in file di configurazione
    if (!controller->connect(ipAddress)) {
        traceErr() << "GalilCNInspector: connessione al CN fallita";
        QTimer::singleShot(reconnectionIntervalMs, this, &GalilCNInspector::startProcess);
    } else
        emit connectedSignal();

    refreshTimer.start();

    emit processStartSignal();
    traceExit;

}

void GalilCNInspector::stopProcess() {

    traceEnter;

    if (refreshTimer.isActive())
        refreshTimer.stop();

    emit processStopSignal();
    traceExit;

}
