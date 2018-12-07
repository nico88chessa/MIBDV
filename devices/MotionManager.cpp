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

int MotionManager::moveX(posType posMm) {

    traceEnter;

    bool check;
    if (!this->isMotorXOff(check))
        return MOTION_MANAGER_ERR_CN_PROBLEM;

    if (check)
        return MOTION_MANAGER_MOTOR_X_OFF;

    if (!this->moveXImpl(posMm))
        return MOTION_MANAGER_ERR_CN_PROBLEM;

    traceExit;
    return MOTION_MANAGER_NO_ERR;

}

void MotionManager::waitAxisXStop() {

    traceEnter;

    traceExit;

}
