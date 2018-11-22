#include "MotionManager.hpp"

#include <AbstractCN.hpp>
#include <Logger.hpp>
#include <Settings.hpp>
#include <GalilCNController.hpp>
#include <QEventLoop>

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

    this->moveXImpl(posMm);

    QEventLoop loop;
    QTimer t;
    t.setInterval(20000);
    int res;
    connect(&t, &QTimer::timeout, [&]() {
        loop.exit(1);
        res = 10;
    });
    connect(this, &MotionManager::axisXStop, &loop, &QEventLoop::exit);
    t.start();
    loop.exec();

    traceExit;
    return true;

}

void MotionManager::waitAxisXStop() {

    traceEnter;

    traceExit;

}
