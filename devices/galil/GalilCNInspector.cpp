#include "GalilCNInspector.hpp"

#include <Logger.hpp>
#include <Utils.hpp>

#include "Settings.hpp"


using namespace PROGRAM_NAMESPACE;

static const ErrorID GALIL_CN_INSPECTOR_FAILED_TO_CONNECT = PROGRAM_ERR_START_CODE + 1;
static const ErrorID GALIL_CN_INSPECTOR_DISCONNECTED = PROGRAM_ERR_START_CODE + 2;

static constexpr char GALIL_CN_INSPECTOR_FAILED_TO_CONNECT_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Impossible to connect to device in Galil CN Inspector");
static constexpr char GALIL_CN_INSPECTOR_DISCONNECTED_DESCR[] = QT_TRANSLATE_NOOP("mibdv", "Galil CN Inspector device disconnected");

GalilCNInspector::GalilCNInspector(QObject* parent) :
    ConnectedDeviceInspector(parent)/*, isFirst(true), isFECheck(0), isCustomHomeAxisX(false) */{

    Settings& s = Settings::instance();
    this->setDevice(new GalilCNController());

    getGalilCNDevicePtr()->setupController(
                s.getGalilCNIpAddress(),
                s.getGalilCNConnectionTimeoutMs(),
                s.getGalilCNNumberDigitalInput(),
                s.getGalilCNNumberDigitalOutput(),
                s.getGalilCNNumberAnalogInput(),
                s.getGalilCNOptionCustomHomeAxisX());

    this->setRefreshValue(s.getGalilCNStatusRefreshIntervalMs());
    this->setRestartTime(s.getGalilCNReconnectionIntervalMs());
//    this->isCustomHomeAxisX = s.getGalilCNOptionCustomHomeAxisX();
    initializeErrorSignaler(errorSignaler, this);

    connect(this, &AbstractConnectedDeviceInspector::failedToConnectSignal, [&]() {
        errorSignaler->clearErrors();
        errorSignaler->addError(Error(DeviceKey::GALIL_CN_INSPECTOR, GALIL_CN_INSPECTOR_FAILED_TO_CONNECT, GALIL_CN_INSPECTOR_FAILED_TO_CONNECT_DESCR, ErrorType::FATAL));
        errorSignaler->notifyErrors();
    });

    connect(this, &AbstractConnectedDeviceInspector::disconnectedSignal, [&]() {
        errorSignaler->clearErrors();
        errorSignaler->addError(Error(DeviceKey::GALIL_CN_INSPECTOR, GALIL_CN_INSPECTOR_DISCONNECTED, GALIL_CN_INSPECTOR_DISCONNECTED_DESCR, ErrorType::FATAL));
        errorSignaler->notifyErrors();
    });

    connect(this, &AbstractConnectedDeviceInspector::connectedSignal, [&]() {
        errorSignaler->clearErrors();
        errorSignaler->notifyErrors();
    });

}

bool GalilCNInspector::connectDevice() {

    traceEnter;
    bool res = getGalilCNDevicePtr()->connect();
    traceExit;
    return res;

}

/*void GalilCNInspector::analizeLastStatus(const MotionInspectorImpl::S& newStatus) {

    traceEnter;

    GalilCNStatusBean oldStatus = lastStatus;
    lastStatus = newStatus;

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
        if (isMotorXOff != oldStatus.getAxisAMotorOff())
            emit axisXMotorOffSignal();

    // check motion asse x
    bool isMovingAxisX = newStatus.getAxisAMoveInProgress();
    if (!isMovingAxisX) {
        if (isMovingAxisX != oldStatus.getAxisAMoveInProgress()) {

            int galilStopCode = newStatus.getAxisAStopCode();

            if (isCustomHomeAxisX) {

                if (galilStopCode == GALIL_CN_STOP_CODE_STOPPED_AFTER_HOMING_FIND_INDEX) {

                    Logger::instance().debug() << "Galil axis X FI ok";
                    emit axisXHomingComplete();
                    isFECheck = 0;

                } else if (galilStopCode == GALIL_CN_STOP_CODE_STOPPED_AFTER_FINDING_EDGE) {

                    Logger::instance().debug() << "Galil axis X FE ok";

                    if (isFECheck < CUSTOM_HOME_AXIS_X_FE_COUNT) {

                        Logger::instance().debug() << "count: " << isFECheck;
                        lastStatus.setAxisAMoveInProgress(true); // obbligo a ricontrollare se l'asse e' davvero fermo al giro dopo
                        ++isFECheck;

                    } else {

                        Logger::instance().debug() << "Galil axis X still in FE";
                        isFECheck = 0;
                        MotionStopCode stopCode = GalilControllerUtils::evaluateStopCode(galilStopCode);
                        if (stopCode != MotionStopCode::MOTION_STOP_CORRECTLY) {
                            Logger::instance().error() << "Galil axis X stop code error:" << galilStopCode;
                            Logger::instance().error() << "Galil axis X stop description:" << GalilControllerUtils::getStopCodeDescription(galilStopCode);
                        }

                        emit axisXMotionStopSignal(stopCode);
                    }

                } else {

                    isFECheck = 0;

                    MotionStopCode stopCode = GalilControllerUtils::evaluateStopCode(galilStopCode);
                    if (stopCode != MotionStopCode::MOTION_STOP_CORRECTLY) {
                        Logger::instance().error() << "Galil axis X stop code error:" << galilStopCode;
                        Logger::instance().error() << "Galil axis X stop description:" << GalilControllerUtils::getStopCodeDescription(galilStopCode);
                    }

                    emit axisXMotionStopSignal(stopCode);
                }

            } else {

                if (galilStopCode == GALIL_CN_STOP_CODE_STOPPED_AFTER_HOMING_FIND_INDEX)
                    emit axisYHomingComplete();

                MotionStopCode stopCode = GalilControllerUtils::evaluateStopCode(galilStopCode);
                if (stopCode != MotionStopCode::MOTION_STOP_CORRECTLY) {
                    Logger::instance().error() << "Galil axis X stop code error:" << galilStopCode;
                    Logger::instance().error() << "Galil axis X stop description:" << GalilControllerUtils::getStopCodeDescription(galilStopCode);
                }
                emit axisXMotionStopSignal(stopCode);

            }

        }
    }

    // check forward limit asse x
    bool isForwardLimitAxisX = newStatus.getAxisAForwardLimit();
    if (isForwardLimitAxisX)
        if (isForwardLimitAxisX != oldStatus.getAxisAForwardLimit())
            emit axisXForwardLimitSignal();

    // check backward limit asse x
    bool isBackwardLimitAxisX = newStatus.getAxisAReverseLimit();
    if (isBackwardLimitAxisX)
        if (isBackwardLimitAxisX != oldStatus.getAxisAReverseLimit())
            emit axisXBackwardLimitSignal();

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
                Logger::instance().error() << "Galil axis Y stop code error:" << galilStopCode;
                Logger::instance().error() << "Galil axis Y stop description:" << GalilControllerUtils::getStopCodeDescription(galilStopCode);
            }
            emit axisYMotionStopSignal(stopCode);
        }
    }

    // check forward limit asse y
    bool isForwardLimitAxisY = newStatus.getAxisBForwardLimit();
    if (isForwardLimitAxisY)
        if (isForwardLimitAxisY != oldStatus.getAxisBForwardLimit())
            emit axisYForwardLimitSignal();

    // check backward limit asse y
    bool isBackwardLimitAxisY = newStatus.getAxisBReverseLimit();
    if (isBackwardLimitAxisY)
        if (isBackwardLimitAxisY != oldStatus.getAxisBReverseLimit())
            emit axisYBackwardLimitSignal();

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
                Logger::instance().error() << "Galil axis Z stop code error:" << galilStopCode;
                Logger::instance().error() << "Galil axis Z stop description:" << GalilControllerUtils::getStopCodeDescription(galilStopCode);
            }
            emit axisZMotionStopSignal(stopCode);
        }
    }

    // check forward limit asse z
    bool isForwardLimitAxisZ = newStatus.getAxisCForwardLimit();
    if (isForwardLimitAxisZ)
        if (isForwardLimitAxisZ != oldStatus.getAxisCForwardLimit())
            emit axisZForwardLimitSignal();

    // check backward limit asse z
    bool isBackwardLimitAxisZ = newStatus.getAxisCReverseLimit();
    if (isBackwardLimitAxisZ)
        if (isBackwardLimitAxisZ != oldStatus.getAxisCReverseLimit())
            emit axisZBackwardLimitSignal();

    // check homing in progress asse z
    bool isHomingZInProgress = newStatus.getAxisCHmInProgress();
    bool lastStatusHomeZInProgress = oldStatus.getAxisCHmInProgress();
    if (isHomingZInProgress != lastStatusHomeZInProgress) {
        if (isHomingZInProgress)
            emit axisZHomeInProgressStartSignal();
        else
            emit axisZHomeInProgressStopSignal();
    }


    traceExit;

}
*/

void GalilCNInspector::handleDisconnection() {
    // do nothing
}
