#include <MotionAnalizer.hpp>
#include <MotionSignaler.hpp>


using namespace PROGRAM_NAMESPACE;

IMotionAnalizer::IMotionAnalizer(QObject* parent) :
    QObject(parent), signaler(nullptr) { }

void IMotionAnalizer::setSignaler(MotionSignaler* const s) {
    this->signaler = s;
}


GalilMotionAnalizer::GalilMotionAnalizer(QObject* parent) :
    AbstractAnalizer(parent), isFirst(true), isFECheck(0), isCustomHomeAxisX(false) {

    Settings& s = Settings::instance();
    this->isCustomHomeAxisX = s.getGalilCNOptionCustomHomeAxisX();
}

void GalilMotionAnalizer::analizeImpl(const GalilCNStatusBean& newStatus) {

    traceEnter;

    GalilCNStatusBean oldStatus = lastStatus;
    lastStatus = newStatus;

    if (isFirst) {
        isFirst = false;
        return;
    }

    // check coppia asse x
    bool isMotorXOff = newStatus.getAxisAMotorOff();
    if (isMotorXOff)
        if (isMotorXOff != oldStatus.getAxisAMotorOff())
            emit signaler->axisXMotorOffSignal();

    // check motion asse x
    bool isMovingAxisX = newStatus.getAxisAMoveInProgress();
    if (!isMovingAxisX) {
        if (isMovingAxisX != oldStatus.getAxisAMoveInProgress()) {

            int galilStopCode = newStatus.getAxisAStopCode();

            if (isCustomHomeAxisX) {

                if (galilStopCode == GALIL_CN_STOP_CODE_STOPPED_AFTER_HOMING_FIND_INDEX) {

                    Logger::instance().debug() << "Galil axis X FI ok";
                    emit signaler->axisXHomingComplete();
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

                        emit signaler->axisXMotionStopSignal(stopCode);
                    }

                } else {

                    isFECheck = 0;

                    MotionStopCode stopCode = GalilControllerUtils::evaluateStopCode(galilStopCode);
                    if (stopCode != MotionStopCode::MOTION_STOP_CORRECTLY) {
                        Logger::instance().error() << "Galil axis X stop code error:" << galilStopCode;
                        Logger::instance().error() << "Galil axis X stop description:" << GalilControllerUtils::getStopCodeDescription(galilStopCode);
                    }

                    emit signaler->axisXMotionStopSignal(stopCode);
                }

            } else {

                if (galilStopCode == GALIL_CN_STOP_CODE_STOPPED_AFTER_HOMING_FIND_INDEX)
                    emit signaler->axisYHomingComplete();

                MotionStopCode stopCode = GalilControllerUtils::evaluateStopCode(galilStopCode);
                if (stopCode != MotionStopCode::MOTION_STOP_CORRECTLY) {
                    Logger::instance().error() << "Galil axis X stop code error:" << galilStopCode;
                    Logger::instance().error() << "Galil axis X stop description:" << GalilControllerUtils::getStopCodeDescription(galilStopCode);
                }
                emit signaler->axisXMotionStopSignal(stopCode);

            }

        }
    }

    // check forward limit asse x
    bool isForwardLimitAxisX = newStatus.getAxisAForwardLimit();
    if (isForwardLimitAxisX)
        if (isForwardLimitAxisX != oldStatus.getAxisAForwardLimit())
            emit signaler->axisXForwardLimitSignal();

    // check backward limit asse x
    bool isBackwardLimitAxisX = newStatus.getAxisAReverseLimit();
    if (isBackwardLimitAxisX)
        if (isBackwardLimitAxisX != oldStatus.getAxisAReverseLimit())
            emit signaler->axisXBackwardLimitSignal();

    // check homing in progress asse x
    bool isHomingXInProgress = newStatus.getAxisAHmInProgress();
    bool lastStatusHomeXInProgress = oldStatus.getAxisAHmInProgress();
    if (isHomingXInProgress != lastStatusHomeXInProgress) {
        if (isHomingXInProgress)
            emit signaler->axisXHomeInProgressStartSignal();
        else
            emit signaler->axisXHomeInProgressStopSignal();
    }

    // check coppia asse y
    bool isMotorYOff = newStatus.getAxisBMotorOff();
    if (isMotorYOff) {
        if (isMotorYOff != oldStatus.getAxisBMotorOff())
            emit signaler->axisYMotorOffSignal();
    }

    // check motion asse y
    bool isMovingAxisY = newStatus.getAxisBMoveInProgress();
    if (!isMovingAxisY) {
        if (isMovingAxisY != oldStatus.getAxisBMoveInProgress()) {

            int galilStopCode = newStatus.getAxisBStopCode();

            if (galilStopCode == GALIL_CN_STOP_CODE_STOPPED_AFTER_HOMING_FIND_INDEX)
                emit signaler->axisYHomingComplete();

            MotionStopCode stopCode = GalilControllerUtils::evaluateStopCode(galilStopCode);
            if (stopCode != MotionStopCode::MOTION_STOP_CORRECTLY) {
                Logger::instance().error() << "Galil axis Y stop code error:" << galilStopCode;
                Logger::instance().error() << "Galil axis Y stop description:" << GalilControllerUtils::getStopCodeDescription(galilStopCode);
            }
            emit signaler->axisYMotionStopSignal(stopCode);
        }
    }

    // check forward limit asse y
    bool isForwardLimitAxisY = newStatus.getAxisBForwardLimit();
    if (isForwardLimitAxisY)
        if (isForwardLimitAxisY != oldStatus.getAxisBForwardLimit())
            emit signaler->axisYForwardLimitSignal();

    // check backward limit asse y
    bool isBackwardLimitAxisY = newStatus.getAxisBReverseLimit();
    if (isBackwardLimitAxisY)
        if (isBackwardLimitAxisY != oldStatus.getAxisBReverseLimit())
            emit signaler->axisYBackwardLimitSignal();

    // check homing in progress asse y
    bool isHomingYInProgress = newStatus.getAxisBHmInProgress();
    bool lastStatusHomeYInProgress = oldStatus.getAxisBHmInProgress();
    if (isHomingYInProgress != lastStatusHomeYInProgress) {
        if (isHomingYInProgress)
            emit signaler->axisYHomeInProgressStartSignal();
        else
            emit signaler->axisYHomeInProgressStopSignal();
    }

    // check coppia asse z
    bool isMotorZOff = newStatus.getAxisCMotorOff();
    if (isMotorZOff) {
        if (isMotorZOff != oldStatus.getAxisCMotorOff())
            emit signaler->axisZMotorOffSignal();
    }

    // check motion asse z
    bool isMovingAxisZ = newStatus.getAxisCMoveInProgress();
    if (!isMovingAxisZ) {
        if (isMovingAxisZ != oldStatus.getAxisCMoveInProgress()) {

            int galilStopCode = newStatus.getAxisCStopCode();

            if (galilStopCode == GALIL_CN_STOP_CODE_STOPPED_AFTER_HOMING_FIND_INDEX)
                emit signaler->axisZHomingComplete();

            MotionStopCode stopCode = GalilControllerUtils::evaluateStopCode(galilStopCode);
            if (stopCode != MotionStopCode::MOTION_STOP_CORRECTLY) {
                Logger::instance().error() << "Galil axis Z stop code error:" << galilStopCode;
                Logger::instance().error() << "Galil axis Z stop description:" << GalilControllerUtils::getStopCodeDescription(galilStopCode);
            }
            emit signaler->axisZMotionStopSignal(stopCode);
        }
    }

    // check forward limit asse z
    bool isForwardLimitAxisZ = newStatus.getAxisCForwardLimit();
    if (isForwardLimitAxisZ)
        if (isForwardLimitAxisZ != oldStatus.getAxisCForwardLimit())
            emit signaler->axisZForwardLimitSignal();

    // check backward limit asse z
    bool isBackwardLimitAxisZ = newStatus.getAxisCReverseLimit();
    if (isBackwardLimitAxisZ)
        if (isBackwardLimitAxisZ != oldStatus.getAxisCReverseLimit())
            emit signaler->axisZBackwardLimitSignal();

    // check homing in progress asse z
    bool isHomingZInProgress = newStatus.getAxisCHmInProgress();
    bool lastStatusHomeZInProgress = oldStatus.getAxisCHmInProgress();
    if (isHomingZInProgress != lastStatusHomeZInProgress) {
        if (isHomingZInProgress)
            emit signaler->axisZHomeInProgressStartSignal();
        else
            emit signaler->axisZHomeInProgressStopSignal();
    }

    traceExit;

}
