#include "GalilCNMotionAnalizer.hpp"
#include <QtCore>


using namespace PROGRAM_NAMESPACE;


static constexpr ErrorID GALIL_CN_MA_AXIS_X_MOTOR_OFF = PROGRAM_ERR_START_CODE + 1;
static constexpr ErrorID GALIL_CN_MA_AXIS_X_FORWARD_LIMIT = PROGRAM_ERR_START_CODE + 2;
static constexpr ErrorID GALIL_CN_MA_AXIS_X_BACKWARD_LIMIT = PROGRAM_ERR_START_CODE + 3;
static constexpr ErrorID GALIL_CN_MOTION_STOP_CODE_START_MASK_X = PROGRAM_ERR_START_CODE + (0x01 << 8); // 65792

static constexpr ErrorID GALIL_CN_MA_AXIS_Y_MOTOR_OFF = PROGRAM_ERR_START_CODE + 21;
static constexpr ErrorID GALIL_CN_MA_AXIS_Y_FORWARD_LIMIT = PROGRAM_ERR_START_CODE + 21;
static constexpr ErrorID GALIL_CN_MA_AXIS_Y_BACKWARD_LIMIT = PROGRAM_ERR_START_CODE + 23;
static constexpr ErrorID GALIL_CN_MOTION_STOP_CODE_START_MASK_Y = PROGRAM_ERR_START_CODE + (0x01 << 9); // 66048

static constexpr ErrorID GALIL_CN_MA_AXIS_Z_MOTOR_OFF = PROGRAM_ERR_START_CODE + 41;
static constexpr ErrorID GALIL_CN_MA_AXIS_Z_FORWARD_LIMIT = PROGRAM_ERR_START_CODE + 42;
static constexpr ErrorID GALIL_CN_MA_AXIS_Z_BACKWARD_LIMIT = PROGRAM_ERR_START_CODE + 43;
static constexpr ErrorID GALIL_CN_MOTION_STOP_CODE_START_MASK_Z = PROGRAM_ERR_START_CODE + (0x01 << 10); // 66560

static constexpr ErrorID GALIL_CN_AXIS_NEED_RESET = PROGRAM_ERR_START_CODE + 60;

static constexpr char GALIL_CN_MA_AXIS_X_MOTOR_OFF_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Axis X motor is off");
static constexpr char GALIL_CN_MA_AXIS_X_FORWARD_LIMIT_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Axis X forward limit");
static constexpr char GALIL_CN_MA_AXIS_X_BACKWARD_LIMIT_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Axis X backward limit");
static constexpr char GALIL_CN_MOTION_STOP_CODE_START_MASK_X_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Axis X Stop code error");

static constexpr char GALIL_CN_MA_AXIS_Y_MOTOR_OFF_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Axis Y motor is off");
static constexpr char GALIL_CN_MA_AXIS_Y_FORWARD_LIMIT_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Axis Y forward limit");
static constexpr char GALIL_CN_MA_AXIS_Y_BACKWARD_LIMIT_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Axis Y backward limit");
static constexpr char GALIL_CN_MOTION_STOP_CODE_START_MASK_Y_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Axis Y Stop code error");

static constexpr char GALIL_CN_MA_AXIS_Z_MOTOR_OFF_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Axis Z motor is off");
static constexpr char GALIL_CN_MA_AXIS_Z_FORWARD_LIMIT_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Axis Z forward limit");
static constexpr char GALIL_CN_MA_AXIS_Z_BACKWARD_LIMIT_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Axis Z backward limit");
static constexpr char GALIL_CN_MOTION_STOP_CODE_START_MASK_Z_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Axis Z Stop code error");

static constexpr char GALIL_CN_AXIS_NEED_RESET_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Need to reset axes");


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

    // check limiti asse x se abilitato
    if (getCheckLimitsAxisX()) {

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

    // check stop code asse x
    int stopCodeAxisX = newStatus.getAxisAStopCode();
    MotionStopCode motionStopCodeAxisX = GalilControllerUtils::evaluateStopCode(stopCodeAxisX);
    QString decodeStopCodeAxisX = GalilControllerUtils::getStopCodeDescription(stopCodeAxisX);
    QString stopCodeAxisXDescr = QString("%1 - %2").arg(tr(GALIL_CN_MOTION_STOP_CODE_START_MASK_X_DESCR)).arg(decodeStopCodeAxisX);
    int stopCodeAxisXErrorCodeToShow = GALIL_CN_MOTION_STOP_CODE_START_MASK_X + stopCodeAxisX;
    if (motionStopCodeAxisX == MotionStopCode::MOTION_STOP_ON_ERROR)
        this->errorSignaler->addError(Error(DeviceKey::MOTION_ANALIZER, stopCodeAxisXErrorCodeToShow, stopCodeAxisXDescr, ErrorType::ERROR));
    else
        this->errorSignaler->addError(Error(DeviceKey::MOTION_ANALIZER, stopCodeAxisXErrorCodeToShow, stopCodeAxisXDescr, ErrorType::INFO));

    // check coppia asse y
    bool isMotorYOff = newStatus.getAxisBMotorOff();
    if (isMotorYOff) {
        if (isMotorYOff != oldStatus.getAxisBMotorOff())
            emit axisYMotorOffSignal();

        if (this->machineStatusR->getCurrentStatus() == MachineStatus::IDLE)
            this->errorSignaler->addError(Error(DeviceKey::MOTION_ANALIZER, GALIL_CN_MA_AXIS_Y_MOTOR_OFF, tr(GALIL_CN_MA_AXIS_Y_MOTOR_OFF_DESCR), ErrorType::WARNING));
        else
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


    // check limiti asse y se abilitato
    if (getCheckLimitsAxisY()) {

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

    // check stop code asse y
    int stopCodeAxisY = newStatus.getAxisBStopCode();
    MotionStopCode motionStopCodeAxisY = GalilControllerUtils::evaluateStopCode(stopCodeAxisY);
    QString decodeStopCodeAxisY = GalilControllerUtils::getStopCodeDescription(stopCodeAxisY);
    QString stopCodeAxisYDescr = QString("%1 - %2").arg(tr(GALIL_CN_MOTION_STOP_CODE_START_MASK_Y_DESCR)).arg(decodeStopCodeAxisY);
    int stopCodeAxisYErrorCodeToShow = GALIL_CN_MOTION_STOP_CODE_START_MASK_Y + stopCodeAxisY;
    if (motionStopCodeAxisY == MotionStopCode::MOTION_STOP_ON_ERROR)
        this->errorSignaler->addError(Error(DeviceKey::MOTION_ANALIZER, stopCodeAxisYErrorCodeToShow, stopCodeAxisYDescr, ErrorType::ERROR));
    else
        this->errorSignaler->addError(Error(DeviceKey::MOTION_ANALIZER, stopCodeAxisYErrorCodeToShow, stopCodeAxisYDescr, ErrorType::INFO));

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

    // check limiti asse z se abilitato
    if (getCheckLimitsAxisX()) {

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

    // check stop code asse z
    int stopCodeAxisZ = newStatus.getAxisCStopCode();
    MotionStopCode motionStopCodeAxisZ = GalilControllerUtils::evaluateStopCode(stopCodeAxisZ);
    QString decodeStopCodeAxisZ = GalilControllerUtils::getStopCodeDescription(stopCodeAxisZ);
    QString stopCodeAxisZDescr = QString("%1 - %2").arg(tr(GALIL_CN_MOTION_STOP_CODE_START_MASK_Z_DESCR)).arg(decodeStopCodeAxisZ);
    int stopCodeAxisZErrorCodeToShow = GALIL_CN_MOTION_STOP_CODE_START_MASK_Z + stopCodeAxisZ;
    if (motionStopCodeAxisZ == MotionStopCode::MOTION_STOP_ON_ERROR)
        this->errorSignaler->addError(Error(DeviceKey::MOTION_ANALIZER, stopCodeAxisZErrorCodeToShow, stopCodeAxisZDescr, ErrorType::ERROR));
    else
        this->errorSignaler->addError(Error(DeviceKey::MOTION_ANALIZER, stopCodeAxisZErrorCodeToShow, stopCodeAxisZDescr, ErrorType::INFO));

    // controllo se e' necessario resettare gli assi
    bool needReset = newStatus.getNeedReset();
    if (needReset)
        this->errorSignaler->addError(Error(DeviceKey::MOTION_ANALIZER, GALIL_CN_AXIS_NEED_RESET, GALIL_CN_AXIS_NEED_RESET_DESCR, ErrorType::WARNING));

    this->errorSignaler->notifyErrors();

    traceExit;

}
