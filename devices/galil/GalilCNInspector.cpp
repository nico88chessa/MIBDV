#include "GalilCNInspector.hpp"

#include <Logger.hpp>

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
            MotionStopCode stopCode = evaluateStopCode(galilStopCode);
            if (stopCode == MotionStopCode::MOTION_STOP_CORRECTLY)
                emit axisXMotionStopSignal();
            else {
                Logger::instance().error() << "Galil axis X stop code error:" << galilStopCode;
                emit axisXMotionStopSignal(stopCode, AbstractMotionUtils::getMotionStopCodeDescription(stopCode) + "\nDetails: " + GalilControllerUtils::getStopCodeDescription(galilStopCode));
            }
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
            MotionStopCode stopCode = evaluateStopCode(galilStopCode);
            if (stopCode == MotionStopCode::MOTION_STOP_CORRECTLY)
                emit axisYMotionStopSignal();
            else {
                Logger::instance().error() << "Galil axis Y stop code error:" << galilStopCode;
                emit axisYMotionStopSignal(stopCode, AbstractMotionUtils::getMotionStopCodeDescription(stopCode) + "\nDetails: " + GalilControllerUtils::getStopCodeDescription(galilStopCode));
            }
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
            MotionStopCode stopCode = evaluateStopCode(galilStopCode);
            if (stopCode == MotionStopCode::MOTION_STOP_CORRECTLY)
                emit axisZMotionStopSignal();
            else {
                Logger::instance().error() << "Galil axis Z stop code error:" << galilStopCode;
                emit axisZMotionStopSignal(stopCode, AbstractMotionUtils::getMotionStopCodeDescription(stopCode) + "\nDetails: " + GalilControllerUtils::getStopCodeDescription(galilStopCode));
            }
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

MotionStopCode GalilCNInspector::evaluateStopCode(int stopCode) {

    MotionStopCode motionStopCode = MotionStopCode::MOTION_NAN;

    switch (stopCode) {
    case GALIL_CN_STOP_CODE_MOTORS_RUNNING_INDEPENDENT_MODE: motionStopCode = MotionStopCode::MOTION_IS_RUNNING; break;
    case GALIL_CN_STOP_CODE_MOTORS_DECELERATING_STOP_COMMANDED_INDEPENDENT_POSITION: motionStopCode = MotionStopCode::MOTION_STOP_CORRECTLY; break;
    case GALIL_CN_STOP_CODE_DECELERATING_STOPPED_FWD_LIMIT_SWITCH_OR_SOFT_LIMIT_FL: motionStopCode = MotionStopCode::MOTION_STOP_ON_ERROR; break;
    case GALIL_CN_STOP_CODE_DECELERATING_STOPPED_REV_LIMIT_SWITCH_OR_SOFT_LIMIT_BL: motionStopCode = MotionStopCode::MOTION_STOP_ON_ERROR; break;
    case GALIL_CN_STOP_CODE_DECELERATING_STOP_COMMAND: motionStopCode = MotionStopCode::MOTION_STOP_COMMAND; break;
    case GALIL_CN_STOP_CODE_STOPPED_ABORT_INPUT: motionStopCode = MotionStopCode::MOTION_STOP_ON_ERROR; break;
    case GALIL_CN_STOP_CODE_STOPPED_ABORT_COMMAND: motionStopCode = MotionStopCode::MOTION_STOP_ON_ERROR; break;
    case GALIL_CN_STOP_CODE_DECELERATING_STOPPED_OFF_ON_ERROR: motionStopCode = MotionStopCode::MOTION_STOP_ON_ERROR; break;
    case GALIL_CN_STOP_CODE_STOPPED_AFTER_FINDING_EDGE: motionStopCode = MotionStopCode::MOTION_STOP_CORRECTLY; break;
    case GALIL_CN_STOP_CODE_STOPPED_AFTER_HOMING_FIND_INDEX: motionStopCode = MotionStopCode::MOTION_STOP_CORRECTLY; break;
    case GALIL_CN_STOP_CODE_STOPPED_SELECTIVE_ABORT_INPUT: motionStopCode = MotionStopCode::MOTION_STOP_ON_ERROR; break;
    case GALIL_CN_STOP_CODE_DECELERATING_STOPPED_ENCODER_FAILURE: motionStopCode = MotionStopCode::MOTION_STOP_ON_ERROR; break;
    case GALIL_CN_STOP_CODE_AMPLIFIER_FAULT: motionStopCode = MotionStopCode::MOTION_STOP_ON_ERROR; break;
    case GALIL_CN_STOP_CODE_STEPPER_POSITION_MAINTENANCE_ERROR: motionStopCode = MotionStopCode::MOTION_STOP_ON_ERROR; break;
    case GALIL_CN_STOP_CODE_RUNNING_IN_PVT_MODE: motionStopCode = MotionStopCode::MOTION_IS_RUNNING; break;
    case GALIL_CN_STOP_CODE_PVT_MODE_COMPLETED_NORMALLY: motionStopCode = MotionStopCode::MOTION_STOP_CORRECTLY; break;
    case GALIL_CN_STOP_CODE_PVT_MODE_EXITED_BECAUSE_BUFFER_IS_EMPTY: motionStopCode = MotionStopCode::MOTION_STOP_ON_ERROR; break;
    case GALIL_CN_STOP_CODE_CONTOUR_RUNNING: motionStopCode = MotionStopCode::MOTION_IS_RUNNING; break;
    case GALIL_CN_STOP_CODE_CONTOUR_STOPPED: motionStopCode = MotionStopCode::MOTION_STOP_CORRECTLY; break;
    case GALIL_CN_STOP_CODE_ECAM_RUNNING: motionStopCode = MotionStopCode::MOTION_IS_RUNNING; break;
    case GALIL_CN_STOP_CODE_ECAM_STOPPED: motionStopCode = MotionStopCode::MOTION_STOP_CORRECTLY; break;
    case GALIL_CN_STOP_CODE_STOPPED_ETHERCAT_COMMUNICATION_FAILURE: motionStopCode = MotionStopCode::MOTION_STOP_ON_ERROR; break;
    case GALIL_CN_STOP_CODE_STOPPED_ETHERCAT_DRIVE_FAULT: motionStopCode = MotionStopCode::MOTION_STOP_ON_ERROR; break;
    case GALIL_CN_STOP_CODE_MC_TIMEOUT: motionStopCode = MotionStopCode::MOTION_STOP_ON_ERROR; break;
    case GALIL_CN_STOP_CODE_VECTOR_SEQUENCE_RUNNING: motionStopCode = MotionStopCode::MOTION_IS_RUNNING; break;
    case GALIL_CN_STOP_CODE_VECTOR_SEQUENCE_STOPPED: motionStopCode = MotionStopCode::MOTION_STOP_CORRECTLY; break;
    }
    return motionStopCode;

}

void GalilCNInspector::handleDisconnection() {
    // do nothing
}
