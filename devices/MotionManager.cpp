#include "MotionManager.hpp"

#include <QEventLoop>
#include <QTimer>

#include <AbstractCN.hpp>
#include <Logger.hpp>
#include <Settings.hpp>
#include <galil/GalilCNController.hpp>

using namespace PROGRAM_NAMESPACE;

MotionManager::MotionManager(QObject* parent) : QObject(parent) {

    traceEnter;

    traceExit;

}

MotionManager::~MotionManager() {

    traceEnter;

    traceExit;

}

MotionErr MotionManager::moveX(posType posMm) {

    traceEnter;

    bool check;
    if (!this->isMotorXOff(check))
        return MOTION_MANAGER_ERR_CN_PROBLEM;

    if (check)
        return MOTION_MANAGER_MOTOR_X_OFF;

    Settings& s = Settings::instance();

    int resolution = s.getAxisXStepPerMm();
    posCNType pos = qRound(posMm * resolution);
    spdCNType speed = qRound(s.getAxisXOperativeSpeedMms() * resolution);
    accCNType acc = qRound(s.getAxisXOperativeAccMms2() * resolution);
    accCNType dec = qRound(s.getAxisXOperativeDecMms2() * resolution);

    if (!this->moveXImpl(pos, speed, acc, dec))
        return MOTION_MANAGER_ERR_CN_PROBLEM;

    traceExit;
    return MOTION_MANAGER_NO_ERR;

}

MotionErr MotionManager::moveXManual(posType posMm) {

    traceEnter;

    bool check;
    if (!this->isMotorXOff(check))
        return MOTION_MANAGER_ERR_CN_PROBLEM;

    if (check)
        return MOTION_MANAGER_MOTOR_X_OFF;

    Settings& s = Settings::instance();

    int resolution = s.getAxisXStepPerMm();
    posCNType pos = qRound(posMm * resolution);
    spdCNType speed = qRound(s.getAxisXManualSpeedMms() * resolution);
    accCNType acc = qRound(s.getAxisXManualAccMms2() * resolution);
    accCNType dec = qRound(s.getAxisXManualDecMms2() * resolution);

    if (!this->moveXImpl(pos, speed, acc, dec))
        return MOTION_MANAGER_ERR_CN_PROBLEM;

    traceExit;
    return MOTION_MANAGER_NO_ERR;

}

MotionErr MotionManager::stopX() {

    traceEnter;
    bool res = this->stopXImpl();
    traceExit;
    if (!res)
        return MOTION_MANAGER_STOP_COMMAND_X_ERR;
    else
        return MOTION_MANAGER_NO_ERR;

}

MotionErr MotionManager::homeX() {

    traceEnter;

    bool check;
    if (!this->isMotorXOff(check))
        return MOTION_MANAGER_ERR_CN_PROBLEM;

    if (check)
        return MOTION_MANAGER_MOTOR_X_OFF;

    Settings& s = Settings::instance();

    int resolution = s.getAxisXStepPerMm();
    spdCNType speed = qRound(s.getAxisXHomingSpeedMms() * resolution);
    accCNType acc = qRound(s.getAxisXHomingAccMms2() * resolution);
    accCNType dec = qRound(s.getAxisXHomingDecMms2() * resolution);

    traceExit;

    if (!this->homeXImpl(speed, acc, dec))
        return MOTION_MANAGER_ERR_CN_PROBLEM;
    else
        return MOTION_MANAGER_NO_ERR;

}

MotionErr MotionManager::moveY(posType posMm) {

    traceEnter;

    bool check;
    if (!this->isMotorYOff(check))
        return MOTION_MANAGER_ERR_CN_PROBLEM;

    if (check)
        return MOTION_MANAGER_MOTOR_Y_OFF;

    Settings& s = Settings::instance();

    int resolution = s.getAxisYStepPerMm();
    posCNType pos = qRound(posMm * resolution);
    spdCNType speed = qRound(s.getAxisYOperativeSpeedMms() * resolution);
    accCNType acc = qRound(s.getAxisYOperativeAccMms2() * resolution);
    accCNType dec = qRound(s.getAxisYOperativeDecMms2() * resolution);

    if (!this->moveYImpl(pos, speed, acc, dec))
        return MOTION_MANAGER_ERR_CN_PROBLEM;

    traceExit;
    return MOTION_MANAGER_NO_ERR;

}

MotionErr MotionManager::moveYManual(posType posMm) {

    traceEnter;

    bool check;
    if (!this->isMotorYOff(check))
        return MOTION_MANAGER_ERR_CN_PROBLEM;

    if (check)
        return MOTION_MANAGER_MOTOR_Y_OFF;

    Settings& s = Settings::instance();

    int resolution = s.getAxisYStepPerMm();
    posCNType pos = qRound(posMm * resolution);
    spdCNType speed = qRound(s.getAxisYManualSpeedMms() * resolution);
    accCNType acc = qRound(s.getAxisYManualAccMms2() * resolution);
    accCNType dec = qRound(s.getAxisYManualDecMms2() * resolution);

    if (!this->moveYImpl(pos, speed, acc, dec))
        return MOTION_MANAGER_ERR_CN_PROBLEM;

    traceExit;
    return MOTION_MANAGER_NO_ERR;

}

MotionErr MotionManager::stopY() {

    traceEnter;
    bool res = this->stopYImpl();
    traceExit;
    if (!res)
        return MOTION_MANAGER_STOP_COMMAND_Y_ERR;
    else
        return MOTION_MANAGER_NO_ERR;

}

MotionErr MotionManager::homeY() {

    traceEnter;

    bool check;
    if (!this->isMotorYOff(check))
        return MOTION_MANAGER_ERR_CN_PROBLEM;

    if (check)
        return MOTION_MANAGER_MOTOR_Y_OFF;

    Settings& s = Settings::instance();

    int resolution = s.getAxisYStepPerMm();
    spdCNType speed = qRound(s.getAxisYHomingSpeedMms() * resolution);
    accCNType acc = qRound(s.getAxisYHomingAccMms2() * resolution);
    accCNType dec = qRound(s.getAxisYHomingDecMms2() * resolution);

    traceExit;

    if (!this->homeYImpl(speed, acc, dec))
        return MOTION_MANAGER_ERR_CN_PROBLEM;
    else
        return MOTION_MANAGER_NO_ERR;

}

MotionErr MotionManager::moveZ(posType posMm) {

    traceEnter;

    bool check;
    if (!this->isMotorZOff(check))
        return MOTION_MANAGER_ERR_CN_PROBLEM;

    if (check)
        return MOTION_MANAGER_MOTOR_Z_OFF;

    Settings& s = Settings::instance();

    int resolution = s.getAxisZStepPerMm();
    posCNType pos = qRound(posMm * resolution);
    spdCNType speed = qRound(s.getAxisZOperativeSpeedMms() * resolution);
    accCNType acc = qRound(s.getAxisZOperativeAccMms2() * resolution);
    accCNType dec = qRound(s.getAxisZOperativeDecMms2() * resolution);

    if (!this->moveZImpl(pos, speed, acc, dec))
        return MOTION_MANAGER_ERR_CN_PROBLEM;

    traceExit;
    return MOTION_MANAGER_NO_ERR;

}

MotionErr MotionManager::moveZManual(posType posMm) {

    traceEnter;

    bool check;
    if (!this->isMotorZOff(check))
        return MOTION_MANAGER_ERR_CN_PROBLEM;

    if (check)
        return MOTION_MANAGER_MOTOR_Z_OFF;

    Settings& s = Settings::instance();

    int resolution = s.getAxisZStepPerMm();
    posCNType pos = qRound(posMm * resolution);
    spdCNType speed = qRound(s.getAxisZManualSpeedMms() * resolution);
    accCNType acc = qRound(s.getAxisZManualAccMms2() * resolution);
    accCNType dec = qRound(s.getAxisZManualDecMms2() * resolution);

    if (!this->moveZImpl(pos, speed, acc, dec))
        return MOTION_MANAGER_ERR_CN_PROBLEM;

    traceExit;
    return MOTION_MANAGER_NO_ERR;

    traceExit;

}

MotionErr MotionManager::stopZ() {

    traceEnter;
    bool res = this->stopZImpl();
    traceExit;

    if (!res)
        return MOTION_MANAGER_STOP_COMMAND_Z_ERR;
    else
        return MOTION_MANAGER_NO_ERR;

}

MotionErr MotionManager::homeZ() {

    traceEnter;

    bool check;
    if (!this->isMotorZOff(check))
        return MOTION_MANAGER_ERR_CN_PROBLEM;

    if (check)
        return MOTION_MANAGER_MOTOR_Z_OFF;

    Settings& s = Settings::instance();

    int resolution = s.getAxisZStepPerMm();
    spdCNType speed = qRound(s.getAxisZHomingSpeedMms() * resolution);
    accCNType acc = qRound(s.getAxisZHomingAccMms2() * resolution);
    accCNType dec = qRound(s.getAxisZHomingDecMms2() * resolution);

    traceExit;

    if (!this->homeZImpl(speed, acc, dec))
        return MOTION_MANAGER_ERR_CN_PROBLEM;
    else
        return MOTION_MANAGER_NO_ERR;

}

MotionErr MotionManager::notifyResetOk() {

    traceEnter;
    traceExit;
    if (!this->notifyResetOkImpl())
        return MOTION_MANAGER_RESET_NOTIFY_KO;
    else
        return MOTION_MANAGER_NO_ERR;

}
