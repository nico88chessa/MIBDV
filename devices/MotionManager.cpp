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
    spdCNType speed = s.getAxisXOperativeSpeedMms() * resolution;
    accCNType acc = s.getAxisXOperativeAccMms2() * resolution;
    accCNType dec = s.getAxisXOperativeDecMms2() * resolution;

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
    spdCNType speed = s.getAxisXManualSpeedMms() * resolution;
    accCNType acc = s.getAxisXManualAccMms2() * resolution;
    accCNType dec = s.getAxisXManualDecMms2() * resolution;

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
    spdCNType speed = s.getAxisYOperativeSpeedMms() * resolution;
    accCNType acc = s.getAxisYOperativeAccMms2() * resolution;
    accCNType dec = s.getAxisYOperativeDecMms2() * resolution;

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
    spdCNType speed = s.getAxisYManualSpeedMms() * resolution;
    accCNType acc = s.getAxisYManualAccMms2() * resolution;
    accCNType dec = s.getAxisYManualDecMms2() * resolution;

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
    spdCNType speed = s.getAxisZOperativeSpeedMms() * resolution;
    accCNType acc = s.getAxisZOperativeAccMms2() * resolution;
    accCNType dec = s.getAxisZOperativeDecMms2() * resolution;

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
    spdCNType speed = s.getAxisZManualSpeedMms() * resolution;
    accCNType acc = s.getAxisZManualAccMms2() * resolution;
    accCNType dec = s.getAxisZManualDecMms2() * resolution;

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
