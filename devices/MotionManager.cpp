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

bool MotionManager::moveX(int posMm) {

    traceEnter;

    if (this->isMotorXOff()) {
        return false;
    }

    if (!this->moveXImpl(posMm)) {
        return false;
    }

    QEventLoop loop;
    QTimer t;
    t.setInterval(10*1000);
    int res;
    connect(&t, &QTimer::timeout, [&]() {
        loop.exit(1);
        res = 10;
    });
    connect(this, &MotionManager::powerOffSignal, &loop, &QEventLoop::quit);
    connect(this, &MotionManager::cycleOffSignal, &loop, &QEventLoop::quit);
    connect(this, &MotionManager::axisXMotorOffSignal, &loop, &QEventLoop::quit);
    t.start();
    loop.exec();

    traceExit;
    return true;

}

void MotionManager::waitAxisXStop() {

    traceEnter;

    traceExit;

}
