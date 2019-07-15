#include "GalilCNMotionAnalizer.hpp"
#include <QtCore>


using namespace PROGRAM_NAMESPACE;

static const ErrorID GALIL_CN_MA_AXIS_X_MOTOR_OFF = PROGRAM_ERR_START_CODE + 1;
static const ErrorID GALIL_CN_MA_AXIS_X_FORWARD_LIMIT = PROGRAM_ERR_START_CODE + 2;
static const ErrorID GALIL_CN_MA_AXIS_X_BACKWARD_LIMIT = PROGRAM_ERR_START_CODE + 3;
static const ErrorID GALIL_CN_MA_AXIS_Y_MOTOR_OFF = PROGRAM_ERR_START_CODE + 4;
static const ErrorID GALIL_CN_MA_AXIS_Y_FORWARD_LIMIT = PROGRAM_ERR_START_CODE + 5;
static const ErrorID GALIL_CN_MA_AXIS_Y_BACKWARD_LIMIT = PROGRAM_ERR_START_CODE + 6;
static const ErrorID GALIL_CN_MA_AXIS_Z_MOTOR_OFF = PROGRAM_ERR_START_CODE + 7;
static const ErrorID GALIL_CN_MA_AXIS_Z_FORWARD_LIMIT = PROGRAM_ERR_START_CODE + 8;
static const ErrorID GALIL_CN_MA_AXIS_Z_BACKWARD_LIMIT = PROGRAM_ERR_START_CODE + 9;

static constexpr char GALIL_CN_MA_AXIS_X_MOTOR_OFF_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Axis X motor is off");
static constexpr char GALIL_CN_MA_AXIS_X_FORWARD_LIMIT_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Axis X forward limit");
static constexpr char GALIL_CN_MA_AXIS_X_BACKWARD_LIMIT_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Axis X backward limit");
static constexpr char GALIL_CN_MA_AXIS_Y_MOTOR_OFF_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Axis Y motor is off");
static constexpr char GALIL_CN_MA_AXIS_Y_FORWARD_LIMIT_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Axis Y forward limit");
static constexpr char GALIL_CN_MA_AXIS_Y_BACKWARD_LIMIT_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Axis Y backward limit");
static constexpr char GALIL_CN_MA_AXIS_Z_MOTOR_OFF_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Axis Z motor is off");
static constexpr char GALIL_CN_MA_AXIS_Z_FORWARD_LIMIT_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Axis Z forward limit");
static constexpr char GALIL_CN_MA_AXIS_Z_BACKWARD_LIMIT_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Axis Z backward limit");


GalilCNMotionAnalizer::GalilCNMotionAnalizer(QObject* parent) :
    AbstractAnalizer(parent), isFirst(true), isFECheck(0), isCustomHomeAxisX(false) {

    Settings& s = Settings::instance();
    this->isCustomHomeAxisX = s.getGalilCNOptionCustomHomeAxisX();
}

void GalilCNMotionAnalizer::analizeImpl(const GalilCNStatusBean& newStatus) {

    traceEnter;

    GalilCNStatusBean oldStatus = lastStatus;
    lastStatus = newStatus;

    if (isFirst) {
        isFirst = false;
        return;
    }

    this->errorSignaler->clearErrors();

    // check coppia asse x
    bool isMotorXOff = newStatus.getAxisAMotorOff();
    if (isMotorXOff) {
        if (isMotorXOff != oldStatus.getAxisAMotorOff())
            emit axisXMotorOffSignal();

        if (this->machineStatusR->getCurrentStatus() == MachineStatus::IDLE)
            this->errorSignaler->addError(Error(DeviceKey::MOTION_ANALIZER, GALIL_CN_MA_AXIS_X_MOTOR_OFF, tr(GALIL_CN_MA_AXIS_X_MOTOR_OFF_DESCR), ErrorType::WARNING));
        else
            this->errorSignaler->addError(Error(DeviceKey::MOTION_ANALIZER, GALIL_CN_MA_AXIS_X_MOTOR_OFF, tr(GALIL_CN_MA_AXIS_X_MOTOR_OFF_DESCR), ErrorType::ERROR));
    }

    // check motion asse x
    bool isMovingAxisX = newStatus.getAxisAMoveInProgress();
    if (!isMovingAxisX) {
        if (isMovingAxisX != oldStatus.getAxisAMoveInProgress()) {

            int galilStopCode = newStatus.getAxisAStopCode();

            if (isCustomHomeAxisX) {

                if (galilStopCode == GALIL_CN_STOP_CODE_STOPPED_AFTER_HOMING_FIND_INDEX) {

                    traceDebug() << "Galil axis X FI ok";
                    emit axisXHomingComplete();
                    isFECheck = 0;

                } else if (galilStopCode == GALIL_CN_STOP_CODE_STOPPED_AFTER_FINDING_EDGE) {

                    traceDebug() << "Galil axis X FE ok";

                    if (isFECheck < CUSTOM_HOME_AXIS_X_FE_COUNT) {

                        traceDebug() << "count: " << isFECheck;
                        lastStatus.setAxisAMoveInProgress(true); // obbligo a ricontrollare se l'asse e' davvero fermo al giro dopo
                        ++isFECheck;

                    } else {

                        traceDebug() << "Galil axis X still in FE";
                        isFECheck = 0;
                        MotionStopCode stopCode = GalilControllerUtils::evaluateStopCode(galilStopCode);
                        if (stopCode != MotionStopCode::MOTION_STOP_CORRECTLY) {
                            traceErr() << "Galil axis X stop code error:" << galilStopCode;
                            traceErr() << "Galil axis X stop description:" << GalilControllerUtils::getStopCodeDescription(galilStopCode);
                        }

                        emit axisXMotionStopSignal(stopCode);
                    }

                } else {

                    isFECheck = 0;

                    MotionStopCode stopCode = GalilControllerUtils::evaluateStopCode(galilStopCode);
                    if (stopCode != MotionStopCode::MOTION_STOP_CORRECTLY) {
                        traceErr() << "Galil axis X stop code error:" << galilStopCode;
                        traceErr() << "Galil axis X stop description:" << GalilControllerUtils::getStopCodeDescription(galilStopCode);
                    }

                    emit axisXMotionStopSignal(stopCode);
                }

            } else {

                if (galilStopCode == GALIL_CN_STOP_CODE_STOPPED_AFTER_HOMING_FIND_INDEX)
                    emit axisXHomingComplete();

                MotionStopCode stopCode = GalilControllerUtils::evaluateStopCode(galilStopCode);
                if (stopCode != MotionStopCode::MOTION_STOP_CORRECTLY) {
                    traceErr() << "Galil axis X stop code error:" << galilStopCode;
                    traceErr() << "Galil axis X stop description:" << GalilControllerUtils::getStopCodeDescription(galilStopCode);
                }
                emit axisXMotionStopSignal(stopCode);

            }

        }
    }

    // check forward limit asse x
    bool isForwardLimitAxisX = newStatus.getAxisAForwardLimit();
    if (isForwardLimitAxisX) {
        if (isForwardLimitAxisX != oldStatus.getAxisAForwardLimit())
            emit axisXForwardLimitSignal();
        this->errorSignaler->addError(Error(DeviceKey::MOTION_ANALIZER, GALIL_CN_MA_AXIS_X_FORWARD_LIMIT, tr(GALIL_CN_MA_AXIS_X_FORWARD_LIMIT_DESCR), ErrorType::ERROR));
    }

    // check backward limit asse x
    bool isBackwardLimitAxisX = newStatus.getAxisAReverseLimit();
    if (isBackwardLimitAxisX) {
        if (isBackwardLimitAxisX != oldStatus.getAxisAReverseLimit())
            emit axisXBackwardLimitSignal();
        this->errorSignaler->addError(Error(DeviceKey::MOTION_ANALIZER, GALIL_CN_MA_AXIS_X_BACKWARD_LIMIT, tr(GALIL_CN_MA_AXIS_X_BACKWARD_LIMIT_DESCR), ErrorType::ERROR));
    }

    // check homing in progress asse x
    bool isHomingXInProgress = newStatus.getAxisAHmInProgress();
    bool lastStatusHomeXInProgress = oldStatus.getAxisAHmInProgress();
    if (isHomingXInProgress != lastStatusHomeXInProgress) {
        if (isHomingXInProgress)
            emit axisXHomeInProgressStartSignal();
        else
            emit axisXHomeInProgressStopSignal();
    }

    // check coppia asse y
    bool isMotorYOff = newStatus.getAxisBMotorOff();
    if (isMotorYOff) {
        if (isMotorYOff != oldStatus.getAxisBMotorOff())
            emit axisYMotorOffSignal();
        this->errorSignaler->addError(Error(DeviceKey::MOTION_ANALIZER, GALIL_CN_MA_AXIS_Y_MOTOR_OFF, tr(GALIL_CN_MA_AXIS_Y_MOTOR_OFF_DESCR), ErrorType::ERROR));
    }

    // check motion asse y
    bool isMovingAxisY = newStatus.getAxisBMoveInProgress();
    if (!isMovingAxisY) {
        if (isMovingAxisY != oldStatus.getAxisBMoveInProgress()) {

            int galilStopCode = newStatus.getAxisBStopCode();

            if (galilStopCode == GALIL_CN_STOP_CODE_STOPPED_AFTER_HOMING_FIND_INDEX)
                emit axisYHomingComplete();

            MotionStopCode stopCode = GalilControllerUtils::evaluateStopCode(galilStopCode);
            if (stopCode != MotionStopCode::MOTION_STOP_CORRECTLY) {
                traceErr() << "Galil axis Y stop code error:" << galilStopCode;
                traceErr() << "Galil axis Y stop description:" << GalilControllerUtils::getStopCodeDescription(galilStopCode);
            }
            emit axisYMotionStopSignal(stopCode);
        }
    }

    // check forward limit asse y
    bool isForwardLimitAxisY = newStatus.getAxisBForwardLimit();
    if (isForwardLimitAxisY) {
        if (isForwardLimitAxisY != oldStatus.getAxisBForwardLimit())
            emit axisYForwardLimitSignal();
        this->errorSignaler->addError(Error(DeviceKey::MOTION_ANALIZER, GALIL_CN_MA_AXIS_Y_FORWARD_LIMIT, tr(GALIL_CN_MA_AXIS_Y_FORWARD_LIMIT_DESCR), ErrorType::ERROR));
    }

    // check backward limit asse y
    bool isBackwardLimitAxisY = newStatus.getAxisBReverseLimit();
    if (isBackwardLimitAxisY) {
        if (isBackwardLimitAxisY != oldStatus.getAxisBReverseLimit())
            emit axisYBackwardLimitSignal();
        this->errorSignaler->addError(Error(DeviceKey::MOTION_ANALIZER, GALIL_CN_MA_AXIS_Y_BACKWARD_LIMIT, tr(GALIL_CN_MA_AXIS_Y_BACKWARD_LIMIT_DESCR), ErrorType::ERROR));
    }

    // check homing in progress asse y
    bool isHomingYInProgress = newStatus.getAxisBHmInProgress();
    bool lastStatusHomeYInProgress = oldStatus.getAxisBHmInProgress();
    if (isHomingYInProgress != lastStatusHomeYInProgress) {
        if (isHomingYInProgress)
            emit axisYHomeInProgressStartSignal();
        else
            emit axisYHomeInProgressStopSignal();
    }

    // check coppia asse z
    bool isMotorZOff = newStatus.getAxisCMotorOff();
    if (isMotorZOff) {
        if (isMotorZOff != oldStatus.getAxisCMotorOff())
            emit axisZMotorOffSignal();

        if (this->machineStatusR->getCurrentStatus() == MachineStatus::IDLE)
            this->errorSignaler->addError(Error(DeviceKey::MOTION_ANALIZER, GALIL_CN_MA_AXIS_Z_MOTOR_OFF, tr(GALIL_CN_MA_AXIS_Z_MOTOR_OFF_DESCR), ErrorType::WARNING));
        else
            this->errorSignaler->addError(Error(DeviceKey::MOTION_ANALIZER, GALIL_CN_MA_AXIS_Z_MOTOR_OFF, tr(GALIL_CN_MA_AXIS_Z_MOTOR_OFF_DESCR), ErrorType::ERROR));
    }

    // check motion asse z
    bool isMovingAxisZ = newStatus.getAxisCMoveInProgress();
    if (!isMovingAxisZ) {
        if (isMovingAxisZ != oldStatus.getAxisCMoveInProgress()) {

            int galilStopCode = newStatus.getAxisCStopCode();

            if (galilStopCode == GALIL_CN_STOP_CODE_STOPPED_AFTER_HOMING_FIND_INDEX)
                emit axisZHomingComplete();

            MotionStopCode stopCode = GalilControllerUtils::evaluateStopCode(galilStopCode);
            if (stopCode != MotionStopCode::MOTION_STOP_CORRECTLY) {
                traceErr() << "Galil axis Z stop code error:" << galilStopCode;
                traceErr() << "Galil axis Z stop description:" << GalilControllerUtils::getStopCodeDescription(galilStopCode);
            }
            emit axisZMotionStopSignal(stopCode);
        }
    }

    // check forward limit asse z
    bool isForwardLimitAxisZ = newStatus.getAxisCForwardLimit();
    if (isForwardLimitAxisZ) {
        if (isForwardLimitAxisZ != oldStatus.getAxisCForwardLimit())
            emit axisZForwardLimitSignal();
        this->errorSignaler->addError(Error(DeviceKey::MOTION_ANALIZER, GALIL_CN_MA_AXIS_Z_FORWARD_LIMIT, tr(GALIL_CN_MA_AXIS_Z_FORWARD_LIMIT_DESCR), ErrorType::ERROR));
    }

    // check backward limit asse z
    bool isBackwardLimitAxisZ = newStatus.getAxisCReverseLimit();
    if (isBackwardLimitAxisZ) {
        if (isBackwardLimitAxisZ != oldStatus.getAxisCReverseLimit())
            emit axisZBackwardLimitSignal();
        this->errorSignaler->addError(Error(DeviceKey::MOTION_ANALIZER, GALIL_CN_MA_AXIS_Z_BACKWARD_LIMIT, tr(GALIL_CN_MA_AXIS_Z_BACKWARD_LIMIT_DESCR), ErrorType::ERROR));
    }

    // check homing in progress asse z
    bool isHomingZInProgress = newStatus.getAxisCHmInProgress();
    bool lastStatusHomeZInProgress = oldStatus.getAxisCHmInProgress();
    if (isHomingZInProgress != lastStatusHomeZInProgress) {
        if (isHomingZInProgress)
            emit axisZHomeInProgressStartSignal();
        else
            emit axisZHomeInProgressStopSignal();
    }

    this->errorSignaler->notifyErrors();

    traceExit;

}
