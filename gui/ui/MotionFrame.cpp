#include "MotionFrame.hpp"
#include "ui_MotionFrame.h"

#include "MotionFrameLogic.hpp"
#include "DialogAlert.hpp"

#include <Logger.hpp>

using namespace PROGRAM_NAMESPACE;

/**********************************************
 *    M O T I O N  F R A M E  L O G I C
 *********************************************/

MotionFrameLogic::MotionFrameLogic() {

}

MotionFrameLogic::~MotionFrameLogic() { }

void MotionFrameLogic::setupLogic(const QSharedPointer<MotionManager>& motionManager) {

    traceEnter;
    this->motionManager = motionManager;
    traceExit;

}

void MotionFrameLogic::moveAxisX() {

    traceEnter;

    QString positionStr = qPtr->ui->dsbPositionX->text();
    posType position = positionStr.toFloat();

    int res;
    res = motionManager->moveX(position);

    if (motionManager->isErr(res)) {

        DialogAlert diag;
        diag.setupLabels("Error", MotionManager::decodeError(res));
        diag.exec();
        return;

    } else {

        QEventLoop loop;
        QTimer t;
        t.setInterval(TIMER_CHECK_MOTION_MS);
        int res = MOTION_MANAGER_NO_ERR;
        QMetaObject::Connection c1 = connect(motionManager.data(), &MotionManager::powerOffSignal, [&]() {
            if (loop.isRunning()) {
                loop.quit();
                res = MOTION_MANAGER_POWER_OFF;
            }
        });
        QMetaObject::Connection c2 = connect(motionManager.data(), &MotionManager::cycleOffSignal, [&]() {
            if (loop.isRunning()) {
                loop.quit();
                res = MOTION_MANAGER_CYCLE_OFF;
            }
        });
        QMetaObject::Connection c3 = connect(motionManager.data(), &MotionManager::axisXMotorOffSignal, [&]() {
            if (loop.isRunning()) {
                loop.quit();
                res = MOTION_MANAGER_MOTOR_X_OFF;
            }
        });
        QMetaObject::Connection c4 = connect(&t, &QTimer::timeout, [&]() {
            if (!qPtr->bean.getAxisXMoveInProgress()) {
                if (loop.isRunning()) {
                    loop.quit();
                    res = MOTION_MANAGER_MOTION_X_STOP_CORRECTLY;
                }
            }
        });
        QMetaObject::Connection c5 = connect(motionManager.data(), &MotionManager::axisXMotionStopSignal, [&]() {
            if (loop.isRunning()) {
                loop.quit();
                res = MOTION_MANAGER_MOTION_X_STOP_CORRECTLY;
            }
        });
        t.start();
        loop.exec();
        t.stop();
        QObject::disconnect(c5);
        QObject::disconnect(c4);
        QObject::disconnect(c3);
        QObject::disconnect(c2);
        QObject::disconnect(c1);

        if (motionManager->isErr(res)) {
            DialogAlert diag;
            diag.setupLabels("Error", MotionManager::decodeError(res));
            diag.exec();
        }

    }



    traceExit;

}


/**********************************************
 *         M O T I O N  F R A M E
 *********************************************/

void MotionFrame::setupSignalsAndSlots() {

    traceEnter;

    connect(ui->pbMoveX, &QPushButton::clicked, dPtr, &MotionFrameLogic::moveAxisX);

    traceExit;

}

void MotionFrame::setupUi() {

    traceEnter;

    ui->setupUi(this);

    Settings& s = Settings::instance();

    ui->dsbAxisXCurrentPosition->setDecimals(3);
    ui->dsbAxisYCurrentPosition->setDecimals(3);
    ui->dsbAxisZCurrentPosition->setDecimals(3);

    ui->dsbAxisXMaxPosition->setValue(s.getAxisXMaxPosMm());
    ui->dsbAxisXMaxPosition->setDecimals(3);
    ui->dsbAxisYMaxPosition->setValue(s.getAxisYMaxPosMm());
    ui->dsbAxisYMaxPosition->setDecimals(3);
    ui->dsbAxisZMaxPosition->setValue(s.getAxisZMaxPosMm());
    ui->dsbAxisZMaxPosition->setDecimals(3);

    int minX = s.getAxisXMinPosMm();
    int maxX = s.getAxisXMaxPosMm();
    int minY = s.getAxisYMinPosMm();
    int maxY = s.getAxisYMaxPosMm();
    int minZ = s.getAxisZMinPosMm();
    int maxZ = s.getAxisZMaxPosMm();

    ui->dsbPositionX->setRange(minX, maxX);
    ui->dsbPositionX->setDecimals(3);
    ui->dsbPositionY->setRange(minY, maxY);
    ui->dsbPositionY->setDecimals(3);
    ui->dsbPositionZ->setRange(minZ, maxZ);
    ui->dsbPositionZ->setDecimals(3);

    ui->pbMoveX->setEnabled(false);
    ui->pbMoveY->setEnabled(false);
    ui->pbMoveZ->setEnabled(false);
    ui->pbStopX->setEnabled(false);
    ui->pbStopY->setEnabled(false);
    ui->pbStopZ->setEnabled(false);

    traceExit;

}

MotionFrame::MotionFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MotionFrame), dPtr(new MotionFrameLogic()) {

    dPtr->qPtr = this;

    this->setupUi();

    this->setupSignalsAndSlots();

}

MotionFrame::~MotionFrame() {
    delete ui;
}

void MotionFrame::setupLogic(const QSharedPointer<MotionManager>& motionManager) {

    traceEnter;

    dPtr->setupLogic(motionManager);

    traceExit;

}

void MotionFrame::updateUI(const MotionBean& b) {

    traceEnter;

    this->bean = b;

    ui->dsbAxisXCurrentPosition->setValue(static_cast<double>(bean.getAxisXPosition()));
    ui->dsbAxisYCurrentPosition->setValue(static_cast<double>(bean.getAxisYPosition()));
    ui->dsbAxisZCurrentPosition->setValue(static_cast<double>(bean.getAxisZPosition()));

    ui->cbAxisXForwardLimit->setChecked(bean.getAxisXForwardLimit());
    ui->cbAxisXMotorOff->setChecked(bean.getAxisXMotorOff());
    ui->cbAxisXMoving->setChecked(bean.getAxisXMoveInProgress());
    ui->cbAxisXReverseLimit->setChecked(bean.getAxisXReverseLimit());

    ui->cbAxisYForwardLimit->setChecked(bean.getAxisYForwardLimit());
    ui->cbAxisYMotorOff->setChecked(bean.getAxisYMotorOff());
    ui->cbAxisYMoving->setChecked(bean.getAxisYMoveInProgress());
    ui->cbAxisYReverseLimit->setChecked(bean.getAxisYReverseLimit());

    ui->cbAxisZForwardLimit->setChecked(bean.getAxisZForwardLimit());
    ui->cbAxisZMotorOff->setChecked(bean.getAxisZMotorOff());
    ui->cbAxisZMoving->setChecked(bean.getAxisZMoveInProgress());
    ui->cbAxisZReverseLimit->setChecked(bean.getAxisZReverseLimit());

    ui->pbMoveX->setEnabled(!bean.getAxisXMoveInProgress());

    traceExit;

}

