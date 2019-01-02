#include "MotionFrame.hpp"
#include "ui_MotionFrame.h"

#include "MotionFrameLogic.hpp"
#include "DialogAlert.hpp"

#include <Logger.hpp>

using namespace PROGRAM_NAMESPACE;

/**********************************************
 *    M O T I O N  F R A M E  L O G I C
 *********************************************/

MotionFrameLogic::MotionFrameLogic() { }

MotionFrameLogic::~MotionFrameLogic() { }

void MotionFrameLogic::setupMotionManager(const QSharedPointer<MotionManager>& motionManager) {

    traceEnter;
    this->motionManager = motionManager;
    traceExit;

}

void MotionFrameLogic::setupIOManager(const QSharedPointer<IOManager>& ioManager) {

    traceEnter;
    this->ioManager = ioManager;
    traceExit;

}

bool MotionFrameLogic::checkCycle() {

    traceEnter;

    bool value;
    value = qPtr->digitalInputStatus.value(IOType::CYCLE).getValue();

    if (!value) {
        DialogAlert diag;
        diag.setupLabels("Error", MotionFrame::decodeError(MotionFrame::MOTION_FRAME_CYCLE_ERR));
        diag.exec();
    }

    return value;
    traceExit;

}

void MotionFrameLogic::moveX() {

    traceEnter;

    if (!this->checkCycle())
        return;

    posType position = qPtr->ui->dsbPositionX->value();

    int res;
    res = motionManager->moveXManual(position);

    if (motionManager->isErr(res)) {

        DialogAlert diag;
        diag.setupLabels("Error", MotionManager::decodeError(res));
        diag.exec();
        return;

    } else {

        QEventLoop loop;
        QTimer t;
        t.setSingleShot(true);
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
            if (!qPtr->motionBean.getAxisXMoveInProgress()) {
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

void MotionFrameLogic::moveY() {

    traceEnter;

    if (!this->checkCycle())
        return;

    posType position = qPtr->ui->dsbPositionY->value();

    int res;
    res = motionManager->moveYManual(position);

    if (motionManager->isErr(res)) {

        DialogAlert diag;
        diag.setupLabels("Error", MotionManager::decodeError(res));
        diag.exec();
        return;

    } else {

        QEventLoop loop;
        QTimer t;
        t.setSingleShot(true);
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
        QMetaObject::Connection c3 = connect(motionManager.data(), &MotionManager::axisYMotorOffSignal, [&]() {
            if (loop.isRunning()) {
                loop.quit();
                res = MOTION_MANAGER_MOTOR_Y_OFF;
            }
        });
        QMetaObject::Connection c4 = connect(&t, &QTimer::timeout, [&]() {
            if (!qPtr->motionBean.getAxisYMoveInProgress()) {
                if (loop.isRunning()) {
                    loop.quit();
                    res = MOTION_MANAGER_MOTION_Y_STOP_CORRECTLY;
                }
            }
            t.stop();
        });
        QMetaObject::Connection c5 = connect(motionManager.data(), &MotionManager::axisYMotionStopSignal, [&]() {
            if (loop.isRunning()) {
                loop.quit();
                res = MOTION_MANAGER_MOTION_Y_STOP_CORRECTLY;
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

void MotionFrameLogic::moveZ() {

    traceEnter;

    if (!this->checkCycle())
        return;

    posType position = qPtr->ui->dsbPositionZ->value();

    int res;
    res = motionManager->moveZManual(position);

    if (motionManager->isErr(res)) {

        DialogAlert diag;
        diag.setupLabels("Error", MotionManager::decodeError(res));
        diag.exec();
        return;

    } else {

        QEventLoop loop;
        QTimer t;
        t.setSingleShot(true);
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
        QMetaObject::Connection c3 = connect(motionManager.data(), &MotionManager::axisZMotorOffSignal, [&]() {
            if (loop.isRunning()) {
                loop.quit();
                res = MOTION_MANAGER_MOTOR_Z_OFF;
            }
        });
        QMetaObject::Connection c4 = connect(&t, &QTimer::timeout, [&]() {
            if (!qPtr->motionBean.getAxisZMoveInProgress()) {
                if (loop.isRunning()) {
                    loop.quit();
                    res = MOTION_MANAGER_MOTION_Z_STOP_CORRECTLY;
                }
            }
            t.stop();
        });
        QMetaObject::Connection c5 = connect(motionManager.data(), &MotionManager::axisZMotionStopSignal, [&]() {
            if (loop.isRunning()) {
                loop.quit();
                res = MOTION_MANAGER_MOTION_Z_STOP_CORRECTLY;
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

void MotionFrameLogic::stopX() {

    traceEnter;

    int res = motionManager->stopX();

    if (motionManager->isErr(res)) {

        DialogAlert diag;
        diag.setupLabels("Error", MotionManager::decodeError(res));
        diag.exec();
        return;

    }

    traceExit;

}

void MotionFrameLogic::stopY() {

    traceEnter;

    int res = motionManager->stopY();

    if (motionManager->isErr(res)) {

        DialogAlert diag;
        diag.setupLabels("Error", MotionManager::decodeError(res));
        diag.exec();
        return;

    }

    traceExit;

}

void MotionFrameLogic::stopZ() {

    traceEnter;

    int res = motionManager->stopZ();

    if (motionManager->isErr(res)) {

        DialogAlert diag;
        diag.setupLabels("Error", MotionManager::decodeError(res));
        diag.exec();
        return;

    }

    traceExit;

}

void MotionFrameLogic::homeAxes() {

    traceEnter;

    if (!this->checkCycle())
        return;

    int res;
    res = motionManager->homeZ();

    if (motionManager->isErr(res)) {

        DialogAlert diag;
        diag.setupLabels("Error", MotionManager::decodeError(res));
        diag.exec();
        return;

    } else {

        qPtr->isHomingAxes = true;
        QEventLoop loop;
        res = MOTION_MANAGER_NO_ERR;
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
        QMetaObject::Connection c3 = connect(motionManager.data(), &MotionManager::axisZMotorOffSignal, [&]() {
            if (loop.isRunning()) {
                loop.quit();
                res = MOTION_MANAGER_MOTOR_Z_OFF;
            }
        });
        QMetaObject::Connection c4 = connect(motionManager.data(), &MotionManager::axisZMotionStopSignal, [&]() {
            if (loop.isRunning()) {
                loop.quit();
                res = MOTION_MANAGER_MOTION_Z_STOP_CORRECTLY;
            }
        });
        QMetaObject::Connection c5 = connect(motionManager.data(), &MotionManager::axisZHomeInProgressStopSignal, [&]() {
            if (loop.isRunning()) {
                loop.quit();
                res = MOTION_MANAGER_HOME_Z_COMPLETED_CORRECTLY;
            }
        });
        loop.exec();
        QObject::disconnect(c5);
        QObject::disconnect(c4);
        QObject::disconnect(c3);
        QObject::disconnect(c2);
        QObject::disconnect(c1);

        if (motionManager->isErr(res)) {
            DialogAlert diag;
            diag.setupLabels("Error", MotionManager::decodeError(res));
            diag.exec();
            qPtr->isHomingAxes = false;
            return;
        }

        if (res != MOTION_MANAGER_HOME_Z_COMPLETED_CORRECTLY) {
            DialogAlert diag;
            diag.setupLabels("Error", tr("Homing Z non completato"));
            diag.exec();
            qPtr->isHomingAxes = false;
            return;
        }

    }

    res = motionManager->homeX();

    if (motionManager->isErr(res)) {

        DialogAlert diag;
        diag.setupLabels("Error", MotionManager::decodeError(res));
        diag.exec();
        qPtr->isHomingAxes = false;
        return;

    }

    res = motionManager->homeY();

    if (motionManager->isErr(res)) {

        DialogAlert diag;
        diag.setupLabels("Error", MotionManager::decodeError(res));
        diag.exec();
        qPtr->isHomingAxes = false;
        return;

    }

    qPtr->isHomingAxes = false;
    traceExit;

}

void MotionFrameLogic::stopAxes() {

    traceEnter;

    int res = motionManager->stopY();
    if (motionManager->isErr(res)) {

        DialogAlert diag;
        diag.setupLabels("Error", MotionManager::decodeError(res));
        diag.exec();
        return;

    }

    res = motionManager->stopX();
    if (motionManager->isErr(res)) {

        DialogAlert diag;
        diag.setupLabels("Error", MotionManager::decodeError(res));
        diag.exec();
        return;

    }

    res = motionManager->stopZ();
    if (motionManager->isErr(res)) {

        DialogAlert diag;
        diag.setupLabels("Error", MotionManager::decodeError(res));
        diag.exec();
        return;

    }

    traceExit;

}

/**********************************************
 *         M O T I O N  F R A M E
 *********************************************/

void MotionFrame::setupSignalsAndSlots() {

    traceEnter;

    connect(ui->pbMoveX, &QPushButton::clicked, dPtr, &MotionFrameLogic::moveX);
    connect(ui->pbMoveY, &QPushButton::clicked, dPtr, &MotionFrameLogic::moveY);
    connect(ui->pbMoveZ, &QPushButton::clicked, dPtr, &MotionFrameLogic::moveZ);

    connect(ui->pbStopX, &QPushButton::clicked, dPtr, &MotionFrameLogic::stopX);
    connect(ui->pbStopY, &QPushButton::clicked, dPtr, &MotionFrameLogic::stopY);
    connect(ui->pbStopZ, &QPushButton::clicked, dPtr, &MotionFrameLogic::stopZ);

    connect(ui->pbResetAxes, &QPushButton::clicked, dPtr, &MotionFrameLogic::homeAxes);
    connect(ui->pbStopAxes, &QPushButton::clicked, dPtr, &MotionFrameLogic::stopAxes);

    traceExit;

}

void MotionFrame::setupUi() {

    traceEnter;

    ui->setupUi(this);

    Settings& s = Settings::instance();

    ui->dsbAxisXMaxPosition->setRange(MOTION_FRAME_DSB_MIN_VALUE, MOTION_FRAME_DSB_MAX_VALUE);
    ui->dsbAxisYMaxPosition->setRange(MOTION_FRAME_DSB_MIN_VALUE, MOTION_FRAME_DSB_MAX_VALUE);
    ui->dsbAxisZMaxPosition->setRange(MOTION_FRAME_DSB_MIN_VALUE, MOTION_FRAME_DSB_MAX_VALUE);
    ui->dsbAxisXCurrentPosition->setRange(MOTION_FRAME_DSB_MIN_VALUE, MOTION_FRAME_DSB_MAX_VALUE);
    ui->dsbAxisYCurrentPosition->setRange(MOTION_FRAME_DSB_MIN_VALUE, MOTION_FRAME_DSB_MAX_VALUE);
    ui->dsbAxisZCurrentPosition->setRange(MOTION_FRAME_DSB_MIN_VALUE, MOTION_FRAME_DSB_MAX_VALUE);

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
    ui->pbStopX->setEnabled(true);
    ui->pbStopY->setEnabled(true);
    ui->pbStopZ->setEnabled(true);

    traceExit;

}

MotionFrame::MotionFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MotionFrame), dPtr(new MotionFrameLogic()),
    isHomingAxes(false) {

    dPtr->qPtr = this;
    this->setupUi();
    this->setupSignalsAndSlots();

}

MotionFrame::~MotionFrame() {
    delete ui;
}

void MotionFrame::setupDevices(const QSharedPointer<PROGRAM_NAMESPACE::MotionManager>& motionManager) {

    traceEnter;
    dPtr->setupMotionManager(motionManager);
    traceExit;

}

void MotionFrame::updateMotionBean(const MotionBean& b) {

    traceEnter;
    this->motionBean = b;
    this->updateUI();
    traceExit;

}

void MotionFrame::updateDigitalInputStatus(const DigitalInputStatus& i) {

    traceEnter;
    this->digitalInputStatus = i;
    traceExit;

}

void MotionFrame::updateUI() {

    traceEnter;

    ui->dsbAxisXCurrentPosition->setValue(static_cast<double>(motionBean.getAxisXPosition()));
    ui->dsbAxisYCurrentPosition->setValue(static_cast<double>(motionBean.getAxisYPosition()));
    ui->dsbAxisZCurrentPosition->setValue(static_cast<double>(motionBean.getAxisZPosition()));

    ui->cbAxisXForwardLimit->setChecked(motionBean.getAxisXForwardLimit());
    ui->cbAxisXMotorOff->setChecked(motionBean.getAxisXMotorOff());
    ui->cbAxisXMoving->setChecked(motionBean.getAxisXMoveInProgress());
    ui->cbAxisXReverseLimit->setChecked(motionBean.getAxisXReverseLimit());

    ui->cbAxisYForwardLimit->setChecked(motionBean.getAxisYForwardLimit());
    ui->cbAxisYMotorOff->setChecked(motionBean.getAxisYMotorOff());
    ui->cbAxisYMoving->setChecked(motionBean.getAxisYMoveInProgress());
    ui->cbAxisYReverseLimit->setChecked(motionBean.getAxisYReverseLimit());

    ui->cbAxisZForwardLimit->setChecked(motionBean.getAxisZForwardLimit());
    ui->cbAxisZMotorOff->setChecked(motionBean.getAxisZMotorOff());
    ui->cbAxisZMoving->setChecked(motionBean.getAxisZMoveInProgress());
    ui->cbAxisZReverseLimit->setChecked(motionBean.getAxisZReverseLimit());

    ui->pbMoveX->setEnabled(!motionBean.getAxisXMoveInProgress() && !isHomingAxes);
    ui->pbMoveY->setEnabled(!motionBean.getAxisYMoveInProgress() && !isHomingAxes);
    ui->pbMoveZ->setEnabled(!motionBean.getAxisZMoveInProgress() && !isHomingAxes);

    ui->pbResetAxes->setEnabled(
                !motionBean.getAxisXMoveInProgress() &&
                !motionBean.getAxisYMoveInProgress() &&
                !motionBean.getAxisZMoveInProgress());

    traceExit;

}

