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

    QString positionStr = qPtr->ui->leAxisXPosition->text();
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
        t.setInterval(10*1000);
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
            if (loop.isRunning()) {
                loop.quit();
                res = MOTION_MANAGER_TIMEOUT;
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
        QObject::disconnect(c1);
        QObject::disconnect(c2);
        QObject::disconnect(c3);
        QObject::disconnect(c4);
        QObject::disconnect(c5);

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

    ui->leAxisXMaxPosition->setText(QString::number(s.getAxisXMaxPosMm(), 'g', 3));
    ui->leAxisYMaxPosition->setText(QString::number(s.getAxisYMaxPosMm(), 'g', 3));
    ui->leAxisZMaxPosition->setText(QString::number(s.getAxisZMaxPosMm(), 'g', 3));

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

void MotionFrame::updateUI(const MotionBean& bean) {

    traceEnter;

    ui->leAxisXCurrentPosition->setText(QString::number(bean.getAxisXPosition(), 'f', 3));
    ui->leAxisYCurrentPosition->setText(QString::number(bean.getAxisYPosition(), 'f', 3));
    ui->leAxisZCurrentPosition->setText(QString::number(bean.getAxisZPosition(), 'f', 3));

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

//    ui->pbMoveX->setEnabled(!bean.getAxisXMoveInProgress());

    traceExit;

}

