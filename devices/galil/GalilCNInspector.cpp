#include "GalilCNInspector.hpp"

#include <Logger.hpp>

using namespace PROGRAM_NAMESPACE;

GalilCNInspector::GalilCNInspector(QObject* parent) :
    MotionInspector(parent), lastStatus(),
    controller(new GalilCNController()),
    refreshTimer(this),
    errorSignaler(new ErrorSignaler(this)), isFirst(true) {

    Settings& s = Settings::instance();

    ipAddress = s.getGalilCNIpAddress();
    reconnectionIntervalMs = s.getGalilCNReconnectionIntervalMs();
    int digitalInput = s.getGalilCNNumberDigitalInput();
    int digitalOutput = s.getGalilCNNumberDigitalOutput();
    int analogInput = s.getGalilCNNumberAnalogInput();

    /*
     * TODO NIC 03/12/2018 : controllare che i segnali Power e Cycle siano
     * cablati sul CN; altrimenti lanciare eccezione;
     */
    powerInput = s.getDigitalInputs().value(IOType::POWER);
    cycleInput = s.getDigitalInputs().value(IOType::CYCLE);

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

void GalilCNInspector::analizeLastStatus(const GalilCNStatusBean& newStatus) {

    traceEnter;

    if (isFirst) {
        isFirst = false;
        return;
    }

    // check power
    bool isPowerOn = newStatus.getDigitalInput(powerInput.getChannel());
    if (isPowerOn != lastStatus.getDigitalInput(powerInput.getChannel())) {
        if (isPowerOn || (!isPowerOn && powerInput.getInvertLogic()))
            emit powerOnSignal();
        else
            emit powerOffSignal();
    }

    // check cycle
    bool isCycleOn = newStatus.getDigitalInput(cycleInput.getChannel());
    if (isCycleOn != lastStatus.getDigitalInput(cycleInput.getChannel())) {
        if (isCycleOn || (!isCycleOn && cycleInput.getInvertLogic()))
            emit cycleOnSignal();
        else
            emit cycleOffSignal();
    }

    // check coppia asse x
    bool isMotorXOff = newStatus.getAxisAMotorOff();
    if (isMotorXOff) {
        if (isMotorXOff != lastStatus.getAxisAMotorOff())
            emit axisXMotorOffSignal();
    }

    // check motion asse x
    bool isMovingAxisX = newStatus.getAxisAMoveInProgress();
    if (!isMovingAxisX) {
        if (isMovingAxisX != lastStatus.getAxisAMoveInProgress())
            emit axisXMotionStopSignal();
    }

    // check coppia asse y
    bool isMotorYOff = newStatus.getAxisBMotorOff();
    if (isMotorYOff) {
        if (isMotorYOff != lastStatus.getAxisBMotorOff())
            emit axisYMotorOffSignal();
    }

    // check motion asse y
    bool isMovingAxisY = newStatus.getAxisBMoveInProgress();
    if (!isMovingAxisY) {
        if (isMovingAxisY != lastStatus.getAxisBMoveInProgress())
            emit axisYMotionStopSignal();
    }

    // check coppia asse z
    bool isMotorZOff = newStatus.getAxisCMotorOff();
    if (isMotorZOff) {
        if (isMotorZOff != lastStatus.getAxisCMotorOff())
            emit axisZMotorOffSignal();
    }

    // check motion asse z
    bool isMovingAxisZ = newStatus.getAxisCMoveInProgress();
    if (!isMovingAxisZ) {
        if (isMovingAxisZ != lastStatus.getAxisCMoveInProgress())
            emit axisZMotionStopSignal();
    }

    traceExit;

}

void GalilCNInspector::process() {

    traceEnter;

    if (controller->isConnected()) {

        try {
            auto status = controller->getStatus();

            analizeLastStatus(status);

            lastStatus = status;
            emit statusSignal(status);

        } catch (NoStatusException& e) {
            Q_UNUSED(e)
            traceErr() << "Errore nel recupero dello stato da GalilCNInspector";
        }


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

    if (!controller->connect(ipAddress)) {
        traceErr() << "GalilCNInspector: connessione al CN fallita";
        QTimer::singleShot(reconnectionIntervalMs, this, &GalilCNInspector::startProcess);
        return;
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
