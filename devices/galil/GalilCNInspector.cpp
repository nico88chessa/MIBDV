#include "GalilCNInspector.hpp"

#include <Logger.hpp>
#include <Utils.hpp>

#include "Settings.hpp"

using namespace PROGRAM_NAMESPACE;

GalilCNInspector::GalilCNInspector(QObject* parent) :
    MotionInspectorImpl(parent), isFirst(true) {

    Settings& s = Settings::instance();
    ipAddress = s.getGalilCNIpAddress();
    this->setDevice(new GalilCNController());

    this->setRefreshValue(s.getGalilCNStatusRefreshIntervalMs());
    this->setRestartTime(s.getGalilCNReconnectionIntervalMs());

}

bool GalilCNInspector::connectDevice() {

    traceEnter;
    bool res = getGalilCNDevicePtr()->connect(ipAddress);
    traceExit;
    return res;

}

void GalilCNInspector::analizeLastStatus(const MotionInspectorImpl::S& newStatus) {

    traceEnter;

    if (isFirst) {
        isFirst = false;
        return;
    }

    // check power
//    bool isPowerOn = newStatus.getDigitalInput(powerInput.getChannel());
//    if (isPowerOn != lastStatus.getDigitalInput(powerInput.getChannel())) {
//        if (isPowerOn || (!isPowerOn && powerInput.getInvertLogic()))
//            emit powerOnSignal();
//        else
//            emit powerOffSignal();
//    }

//    // check cycle
//    bool isCycleOn = newStatus.getDigitalInput(cycleInput.getChannel());
//    if (isCycleOn != lastStatus.getDigitalInput(cycleInput.getChannel())) {
//        if (isCycleOn || (!isCycleOn && cycleInput.getInvertLogic()))
//            emit cycleOnSignal();
//        else
//            emit cycleOffSignal();
//    }

    // check coppia asse x
    bool isMotorXOff = newStatus.getAxisAMotorOff();
    if (isMotorXOff)
        if (isMotorXOff != lastStatus.getAxisAMotorOff())
            emit axisXMotorOffSignal();

    // check motion asse x
    bool isMovingAxisX = newStatus.getAxisAMoveInProgress();
    if (!isMovingAxisX)
        if (isMovingAxisX != lastStatus.getAxisAMoveInProgress()) {
            int galilStopCode = newStatus.getAxisAStopCode();
            MotionStopCode stopCode = GalilControllerUtils::evaluateStopCode(galilStopCode);
            if (stopCode != MotionStopCode::MOTION_STOP_CORRECTLY) {
                Logger::instance().error() << "Galil axis X stop code error:" << galilStopCode;
                Logger::instance().error() << "Galil axis X stop description:" << GalilControllerUtils::getStopCodeDescription(galilStopCode);
            }
            emit axisXMotionStopSignal(stopCode);
        }

    // check forward limit asse x
    bool isForwardLimitAxisX = newStatus.getAxisAForwardLimit();
    if (isForwardLimitAxisX)
        if (isForwardLimitAxisX != lastStatus.getAxisAForwardLimit())
            emit axisXForwardLimitSignal();

    // check backward limit asse x
    bool isBackwardLimitAxisX = newStatus.getAxisAReverseLimit();
    if (isBackwardLimitAxisX)
        if (isBackwardLimitAxisX != lastStatus.getAxisAReverseLimit())
            emit axisXBackwardLimitSignal();

    // check homing in progress asse x
    bool isHomingXInProgress = newStatus.getAxisAHmInProgress();
    bool lastStatusHomeXInProgress = lastStatus.getAxisAHmInProgress();
    if (isHomingXInProgress != lastStatusHomeXInProgress) {
        if (isHomingXInProgress)
            emit axisXHomeInProgressStartSignal();
        else
            emit axisXHomeInProgressStopSignal();
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
        if (isMovingAxisY != lastStatus.getAxisBMoveInProgress()) {
            int galilStopCode = newStatus.getAxisBStopCode();
            MotionStopCode stopCode = GalilControllerUtils::evaluateStopCode(galilStopCode);
            if (stopCode != MotionStopCode::MOTION_STOP_CORRECTLY) {
                Logger::instance().error() << "Galil axis Y stop code error:" << galilStopCode;
                Logger::instance().error() << "Galil axis X stop description:" << GalilControllerUtils::getStopCodeDescription(galilStopCode);
            }
            emit axisYMotionStopSignal(stopCode);
        }
    }

    // check forward limit asse y
    bool isForwardLimitAxisY = newStatus.getAxisBForwardLimit();
    if (isForwardLimitAxisY)
        if (isForwardLimitAxisY != lastStatus.getAxisBForwardLimit())
            emit axisYForwardLimitSignal();

    // check backward limit asse y
    bool isBackwardLimitAxisY = newStatus.getAxisBReverseLimit();
    if (isBackwardLimitAxisY)
        if (isBackwardLimitAxisY != lastStatus.getAxisBReverseLimit())
            emit axisYBackwardLimitSignal();

    // check homing in progress asse y
    bool isHomingYInProgress = newStatus.getAxisBHmInProgress();
    bool lastStatusHomeYInProgress = lastStatus.getAxisBHmInProgress();
    if (isHomingYInProgress != lastStatusHomeYInProgress) {
        if (isHomingYInProgress)
            emit axisYHomeInProgressStartSignal();
        else
            emit axisYHomeInProgressStopSignal();
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
        if (isMovingAxisZ != lastStatus.getAxisCMoveInProgress()) {
            int galilStopCode = newStatus.getAxisCStopCode();
            MotionStopCode stopCode = GalilControllerUtils::evaluateStopCode(galilStopCode);
            if (stopCode != MotionStopCode::MOTION_STOP_CORRECTLY) {
                Logger::instance().error() << "Galil axis Z stop code error:" << galilStopCode;
                Logger::instance().error() << "Galil axis X stop description:" << GalilControllerUtils::getStopCodeDescription(galilStopCode);
            }
            emit axisZMotionStopSignal(stopCode);
        }
    }

    // check forward limit asse z
    bool isForwardLimitAxisZ = newStatus.getAxisCForwardLimit();
    if (isForwardLimitAxisZ)
        if (isForwardLimitAxisZ != lastStatus.getAxisCForwardLimit())
            emit axisZForwardLimitSignal();

    // check backward limit asse z
    bool isBackwardLimitAxisZ = newStatus.getAxisCReverseLimit();
    if (isBackwardLimitAxisZ)
        if (isBackwardLimitAxisZ != lastStatus.getAxisCReverseLimit())
            emit axisZBackwardLimitSignal();

    // check homing in progress asse z
    bool isHomingZInProgress = newStatus.getAxisCHmInProgress();
    bool lastStatusHomeZInProgress = lastStatus.getAxisCHmInProgress();
    if (isHomingZInProgress != lastStatusHomeZInProgress) {
        if (isHomingZInProgress)
            emit axisZHomeInProgressStartSignal();
        else
            emit axisZHomeInProgressStopSignal();
    }

    lastStatus = newStatus;
    traceExit;

}

void GalilCNInspector::handleDisconnection() {
    // do nothing
}
