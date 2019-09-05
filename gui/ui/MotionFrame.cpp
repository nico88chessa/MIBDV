#include "MotionFrame.hpp"
#include "ui_MotionFrame.h"

#include "MotionFrameLogic.hpp"
#include "DialogAlert.hpp"

#include <DeviceFactory.hpp>
#include <IOSignaler.hpp>

#include <Logger.hpp>


using namespace PROGRAM_NAMESPACE;

/**********************************************
 *    M O T I O N  F R A M E  L O G I C
 *********************************************/

MotionFrameLogic::MotionFrameLogic() { }

MotionFrameLogic::~MotionFrameLogic() { }

void MotionFrameLogic::initialize() {
    traceEnter;

    this->motionAnalizer = DeviceFactoryInstance.getMotionAnalizer();
    this->ioSignaler = DeviceFactoryInstance.getIOSignaler();
    this->motionManager = DeviceFactoryInstance.instanceMotionManager();
    this->ioManager = DeviceFactoryInstance.instanceIOManager();


    if (!this->motionManager->isConnected())
        this->motionManager->connect();

    if (!this->ioManager->isConnected())
        this->ioManager->connect();


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

    traceInfo() << "Inizio move asse X";

    if (!this->checkCycle())
        return;

    posType position = qPtr->ui->dsbPositionX->value();

    int res;
    res = motionManager->moveXManual(position);

    if (motionManager->isErr(res)) {

        QString descrErr = MotionManager::decodeError(res);
        traceErr() << "Errore comando move asse X - codice:" << res;
        traceErr() << "Descrizione:" << descrErr;

        DialogAlert diag;
        diag.setupLabels("Error", descrErr);
        diag.exec();
        return;

    } else {

        QEventLoop loop;
        QTimer t;
        t.setSingleShot(true);
        t.setInterval(TIMER_CHECK_MOTION_MS);
        int res = MOTION_MANAGER_NO_ERR;

        QMetaObject::Connection c1 = connect(&t, &QTimer::timeout, [&]() {
            if (!qPtr->motionBean.getAxisXMoveInProgress()) {
                if (loop.isRunning()) {
                    res = MOTION_MANAGER_MOTION_X_STOP_CORRECTLY;
                    loop.quit();
                }
            }
        });
        QMetaObject::Connection c2 = connect(motionAnalizer.data(), static_cast<void (IMotionAnalizer::*)(MotionStopCode)>(&IMotionAnalizer::axisXMotionStopSignal), [&](MotionStopCode sc) {
            if (loop.isRunning()) {
                if (sc == MotionStopCode::MOTION_STOP_CORRECTLY)
                    res = MOTION_MANAGER_MOTION_X_STOP_CORRECTLY;
                else
                    res = MOTION_MANAGER_MOTION_X_STOP_ERROR;
                loop.quit();
            }
        });

        t.start();
        loop.exec();
        t.stop();
        QObject::disconnect(c1);
        QObject::disconnect(c2);

        if (motionManager->isErr(res)) {

            QString descrErr = MotionManager::decodeError(res);
            traceErr() << "Errore move asse X - codice:" << res;
            traceErr() << "Descrizione:" << descrErr;

            DialogAlert diag;
            diag.setupLabels("Error", descrErr);
            diag.exec();
        }

    }

    traceExit;

}

void MotionFrameLogic::moveY() {

    traceEnter;

    traceInfo() << "Inizio move asse Y";

    if (!this->checkCycle())
        return;

    posType position = qPtr->ui->dsbPositionY->value();

    int res;
    res = motionManager->moveYManual(position);

    if (motionManager->isErr(res)) {

        QString descrErr = MotionManager::decodeError(res);
        traceErr() << "Errore comando move asse Y - codice:" << res;
        traceErr() << "Descrizione:" << descrErr;

        DialogAlert diag;
        diag.setupLabels("Error", descrErr);
        diag.exec();
        return;

    } else {
    
        QEventLoop loop;
        QTimer t;
        t.setSingleShot(true);
        t.setInterval(TIMER_CHECK_MOTION_MS);
        int res = MOTION_MANAGER_NO_ERR;

        QMetaObject::Connection c1 = connect(&t, &QTimer::timeout, [&]() {
            if (!qPtr->motionBean.getAxisYMoveInProgress()) {
                if (loop.isRunning()) {
                    res = MOTION_MANAGER_MOTION_Y_STOP_CORRECTLY;
                    loop.quit();
                }
            }
            t.stop();
        });
        QMetaObject::Connection c2 = connect(motionAnalizer.data(), static_cast<void (IMotionAnalizer::*)(MotionStopCode)>(&IMotionAnalizer::axisYMotionStopSignal), [&](MotionStopCode sc) {
            if (loop.isRunning()) {
                if (sc == MotionStopCode::MOTION_STOP_CORRECTLY)
                    res = MOTION_MANAGER_MOTION_Y_STOP_CORRECTLY;
                else
                    res = MOTION_MANAGER_MOTION_Y_STOP_ERROR;
                loop.quit();
            }
        });

        t.start();
        loop.exec();
        t.stop();
        QObject::disconnect(c1);
        QObject::disconnect(c2);

        if (motionManager->isErr(res)) {

            QString descrErr = MotionManager::decodeError(res);
            traceErr() << "Errore move asse Y - codice:" << res;
            traceErr() << "Descrizione:" << descrErr;

            DialogAlert diag;
            diag.setupLabels("Error", descrErr);
            diag.exec();
        }

    }

    traceExit;

}

void MotionFrameLogic::moveZ() {

    traceEnter;

    traceInfo() << "Inizio move asse Z";

    if (!this->checkCycle())
        return;

    posType position = qPtr->ui->dsbPositionZ->value();

    int res;
    res = motionManager->moveZManual(position);

    if (motionManager->isErr(res)) {

        QString descrErr = MotionManager::decodeError(res);
        traceErr() << "Errore comando move asse Z - codice:" << res;
        traceErr() << "Descrizione:" << descrErr;

        DialogAlert diag;
        diag.setupLabels("Error", descrErr);
        diag.exec();
        return;

    } else {

        QString stopErrorDescription;
        QEventLoop loop;
        QTimer t;
        t.setSingleShot(true);
        t.setInterval(TIMER_CHECK_MOTION_MS);
        int res = MOTION_MANAGER_NO_ERR;

        QMetaObject::Connection c1 = connect(&t, &QTimer::timeout, [&]() {
            if (!qPtr->motionBean.getAxisZMoveInProgress()) {
                if (loop.isRunning()) {
                    res = MOTION_MANAGER_MOTION_Z_STOP_CORRECTLY;
                    loop.quit();
                }
            }
            t.stop();
        });
        QMetaObject::Connection c2 = connect(motionAnalizer.data(), static_cast<void (IMotionAnalizer::*)(MotionStopCode)>(&IMotionAnalizer::axisZMotionStopSignal), [&](MotionStopCode sc) {
            if (loop.isRunning()) {
                if (sc == MotionStopCode::MOTION_STOP_CORRECTLY)
                    res = MOTION_MANAGER_MOTION_Z_STOP_CORRECTLY;
                else
                    res = MOTION_MANAGER_MOTION_Z_STOP_ERROR;
                loop.quit();
            }
        });

        t.start();
        loop.exec();
        t.stop();
        QObject::disconnect(c1);
        QObject::disconnect(c2);

        if (motionManager->isErr(res)) {

            QString descrErr = MotionManager::decodeError(res);
            traceErr() << "Errore move asse Z - codice:" << res;
            traceErr() << "Descrizione:" << descrErr;

            DialogAlert diag;
            diag.setupLabels("Error", descrErr);
            diag.exec();
        }

    }

    traceExit;

}

void MotionFrameLogic::stopX() {

    traceEnter;

    int res = motionManager->stopX();

    if (motionManager->isErr(res)) {

        QString descrErr = MotionManager::decodeError(res);
        traceErr() << "Errore comando stop asse X - codice:" << res;
        traceErr() << "Descrizione:" << descrErr;

        DialogAlert diag;
        diag.setupLabels("Error", descrErr);
        diag.exec();
        return;

    }

    traceExit;

}

void MotionFrameLogic::stopY() {

    traceEnter;

    int res = motionManager->stopY();

    if (motionManager->isErr(res)) {

        QString descrErr = MotionManager::decodeError(res);
        traceErr() << "Errore comando stop asse Y - codice:" << res;
        traceErr() << "Descrizione:" << descrErr;

        DialogAlert diag;
        diag.setupLabels("Error", descrErr);
        diag.exec();
        return;

    }

    traceExit;

}

void MotionFrameLogic::stopZ() {

    traceEnter;

    int res = motionManager->stopZ();

    if (motionManager->isErr(res)) {

        QString descrErr = MotionManager::decodeError(res);
        traceErr() << "Errore comando stop asse Z - codice:" << res;
        traceErr() << "Descrizione:" << descrErr;

        DialogAlert diag;
        diag.setupLabels("Error", descrErr);
        diag.exec();
        return;

    }

    traceExit;

}

void MotionFrameLogic::homeAxes() {

    traceEnter;

    traceInfo() << "Inizio procedura homing assi";

    if (!this->checkCycle())
        return;

    int res;
    res = motionManager->homeZ();

    if (motionManager->isErr(res)) {

        QString descrErr = MotionManager::decodeError(res);
        traceErr() << "Errore comando home asse Z - codice:" << res;
        traceErr() << "Descrizione:" << descrErr;

        DialogAlert diag;
        diag.setupLabels("Error", descrErr);
        diag.exec();
        return;

    } else {

        qPtr->isHomingAxes = true;
        QEventLoop loop;
        QTimer t;
        t.setSingleShot(true);
        t.setInterval(TIMER_CHECK_MOTION_MS);
        res = MOTION_MANAGER_NO_ERR;

        QMetaObject::Connection c1 = connect(motionAnalizer.data(), &IMotionAnalizer::axisZHomingComplete, [&]{
            if (loop.isRunning()) {
                res = MOTION_MANAGER_MOTION_Z_STOP_CORRECTLY;
                loop.quit();
            }
        });
        QMetaObject::Connection c2 = connect(&t, &QTimer::timeout, [&]() {
            if (loop.isRunning()) {
                // controllo se l'asse si sta muovendo
                if (!qPtr->motionBean.getAxisZMoveInProgress()) {
                    // se e' fermo, controllo come si e' fermato controllando il codice di errore
                    if (qPtr->motionBean.getAxisZStopCode() == MotionStopCode::MOTION_STOP_CORRECTLY)
                        res = MOTION_MANAGER_MOTION_Z_STOP_CORRECTLY;
                    else
                        res = MOTION_MANAGER_MOTION_Z_STOP_ERROR;
                    loop.quit();
                }
            }
        });
        QMetaObject::Connection c3 = connect(motionAnalizer.data(), static_cast<void (IMotionAnalizer::*)(MotionStopCode)>(&IMotionAnalizer::axisZMotionStopSignal), [&](MotionStopCode sc) {
            if (loop.isRunning()) {
                if (sc == MotionStopCode::MOTION_STOP_CORRECTLY)
                    res = MOTION_MANAGER_MOTION_Z_STOP_CORRECTLY;
                else
                    res = MOTION_MANAGER_MOTION_Z_STOP_ERROR;
                loop.quit();
            }
        });

        t.start();
        loop.exec();
        t.stop();
        QObject::disconnect(c1);
        QObject::disconnect(c2);
        QObject::disconnect(c3);

        if (motionManager->isErr(res)) {

            QString descrErr = MotionManager::decodeError(res);
            traceErr() << "Errore home asse Z - codice:" << res;
            traceErr() << "Descrizione:" << descrErr;

            DialogAlert diag;
            diag.setupLabels("Error", descrErr);
            diag.exec();
            qPtr->isHomingAxes = false;
            return;
        }

    }

    res = motionManager->homeX();

    if (motionManager->isErr(res)) {

        QString descrErr = MotionManager::decodeError(res);
        traceErr() << "Errore comando home asse X - codice:" << res;
        traceErr() << "Descrizione:" << descrErr;

        DialogAlert diag;
        diag.setupLabels("Error", descrErr);
        diag.exec();
        qPtr->isHomingAxes = false;
        return;

    }

    res = motionManager->homeY();

    if (motionManager->isErr(res)) {

        QString descrErr = MotionManager::decodeError(res);
        traceErr() << "Errore comando home asse Y - codice:" << res;
        traceErr() << "Descrizione:" << descrErr;

        DialogAlert diag;
        diag.setupLabels("Error", descrErr);
        diag.exec();
        qPtr->isHomingAxes = false;
        return;

    }

    QEventLoop loop;
    QTimer tx, ty;
    tx.setSingleShot(true);
    tx.setInterval(TIMER_CHECK_MOTION_MS);
    ty.setSingleShot(true);
    ty.setInterval(TIMER_CHECK_MOTION_MS);
    int resX = MOTION_MANAGER_NO_ERR;
    int resY = MOTION_MANAGER_NO_ERR;
    bool isAxisXMoving = true;
    bool isAxisYMoving = true;


    QMetaObject::Connection c1 = connect(motionAnalizer.data(), &IMotionAnalizer::axisXHomingComplete, [&]{
        if (loop.isRunning()) {
            isAxisXMoving = false;
            resX = MOTION_MANAGER_MOTION_X_STOP_CORRECTLY;
            if (!(isAxisXMoving || isAxisYMoving))
                loop.quit();
        }
    });
    QMetaObject::Connection c2 = connect(motionAnalizer.data(), &IMotionAnalizer::axisYHomingComplete, [&]{
        if (loop.isRunning()) {
            isAxisYMoving = false;
            resY = MOTION_MANAGER_MOTION_Y_STOP_CORRECTLY;
            if (!(isAxisXMoving || isAxisYMoving))
                loop.quit();
        }
    });
    QMetaObject::Connection c3 = connect(&tx, &QTimer::timeout, [&]() {
        if (loop.isRunning()) {
            // controllo se l'asse si sta muovendo
            if (!qPtr->motionBean.getAxisXMoveInProgress()) {
                isAxisXMoving = false;
                // se e' fermo, controllo come si e' fermato controllando il codice di errore
                if (qPtr->motionBean.getAxisXStopCode() == MotionStopCode::MOTION_STOP_CORRECTLY)
                    resX = MOTION_MANAGER_MOTION_X_STOP_CORRECTLY;
                else
                    resX = MOTION_MANAGER_MOTION_X_STOP_ERROR;
                if (!(isAxisXMoving || isAxisYMoving))
                    loop.quit();
            }
        }
    });
    QMetaObject::Connection c4 = connect(&ty, &QTimer::timeout, [&]() {
        if (loop.isRunning()) {
            // controllo se l'asse si sta muovendo
            if (!qPtr->motionBean.getAxisYMoveInProgress()) {
                isAxisYMoving = false;
                // se e' fermo, controllo come si e' fermato controllando il codice di errore
                if (qPtr->motionBean.getAxisYStopCode() == MotionStopCode::MOTION_STOP_CORRECTLY)
                    resY = MOTION_MANAGER_MOTION_Y_STOP_CORRECTLY;
                else
                    resY = MOTION_MANAGER_MOTION_Y_STOP_ERROR;
                if (!(isAxisXMoving || isAxisYMoving))
                    loop.quit();
            }
        }
    });
    QMetaObject::Connection c5 = connect(motionAnalizer.data(), static_cast<void (IMotionAnalizer::*)(MotionStopCode)>(&IMotionAnalizer::axisXMotionStopSignal), [&](MotionStopCode sc) {
        if (loop.isRunning()) {
            isAxisXMoving = false;
            if (sc == MotionStopCode::MOTION_STOP_CORRECTLY)
                resX = MOTION_MANAGER_MOTION_X_STOP_CORRECTLY;
            else
                resX = MOTION_MANAGER_MOTION_X_STOP_ERROR;
            if (!(isAxisXMoving || isAxisYMoving))
                loop.quit();
        }
    });
    QMetaObject::Connection c6 = connect(motionAnalizer.data(), static_cast<void (IMotionAnalizer::*)(MotionStopCode)>(&IMotionAnalizer::axisYMotionStopSignal), [&](MotionStopCode sc) {
        if (loop.isRunning()) {
            isAxisYMoving = false;
            if (sc == MotionStopCode::MOTION_STOP_CORRECTLY)
                resY = MOTION_MANAGER_MOTION_Y_STOP_CORRECTLY;
            else
                resY = MOTION_MANAGER_MOTION_Y_STOP_ERROR;
            if (!(isAxisXMoving || isAxisYMoving))
                loop.quit();
        }
    });

    tx.start();
    ty.start();
    loop.exec();
    tx.stop();
    ty.stop();

    QObject::disconnect(c1);
    QObject::disconnect(c2);
    QObject::disconnect(c3);
    QObject::disconnect(c4);
    QObject::disconnect(c5);
    QObject::disconnect(c6);

    if (motionManager->isErr(resX)) {

        QString descrErr = MotionManager::decodeError(resX);
        traceErr() << "Errore home asse Z - codice:" << resX;
        traceErr() << "Descrizione:" << descrErr;

        DialogAlert diag;
        diag.setupLabels("Error", descrErr);
        diag.exec();
        qPtr->isHomingAxes = false;
        return;
    }

    if (motionManager->isErr(resY)) {

        QString descrErr = MotionManager::decodeError(resY);
        traceErr() << "Errore home asse Z - codice:" << resY;
        traceErr() << "Descrizione:" << descrErr;

        DialogAlert diag;
        diag.setupLabels("Error", descrErr);
        diag.exec();
        qPtr->isHomingAxes = false;
        return;
    }

    qPtr->isHomingAxes = false;

    res = motionManager->notifyResetOk();
    if (motionManager->isErr(res)) {

        QString descrErr = MotionManager::decodeError(res);
        traceErr() << "Errore segnalazione home OK al device - codice:" << res;
        traceErr() << "Descrizione:" << descrErr;

        DialogAlert diag;
        diag.setupLabels("Error", descrErr);
        diag.exec();
        return;

    }

    traceExit;

}

void MotionFrameLogic::stopAxes() {

    traceEnter;

    traceInfo() << "Inizio procedura stop assi";

    int res = motionManager->stopY();
    if (motionManager->isErr(res)) {

        QString descr = MotionManager::decodeError(res);
        traceErr() << "Errore comando stop asse Y - codice:" << res;
        traceErr() << "Descrizione:" << descr;

        DialogAlert diag;
        diag.setupLabels("Error", descr);
        diag.exec();
        return;

    }

    res = motionManager->stopX();
    if (motionManager->isErr(res)) {

        QString descr = MotionManager::decodeError(res);
        traceErr() << "Errore comando stop asse X - codice:" << res;
        traceErr() << "Descrizione:" << descr;

        DialogAlert diag;
        diag.setupLabels("Error", descr);
        diag.exec();
        return;

    }

    res = motionManager->stopZ();
    if (motionManager->isErr(res)) {

        QString descr = MotionManager::decodeError(res);
        traceErr() << "Errore comando stop asse Z - codice:" << res;
        traceErr() << "Descrizione:" << descr;

        DialogAlert diag;
        diag.setupLabels("Error", descr);
        diag.exec();
        return;

    }

    traceExit;

}


/*
 * M O T I O N  F R A M E
 */

void MotionFrame::setupSignalsAndSlots() {

    traceEnter;

    connect(dPtr->motionAnalizer.data(), &IMotionAnalizer::motionBeanSignal, [&](const MotionBean& motionBean) {
        QMetaObject::invokeMethod(this, "updateMotionBean", Qt::QueuedConnection,
                                  Q_ARG(const MotionBean&, motionBean));
    });

//    connect(dPtr->ioSignaler.data(), &IOSignaler::statusSignal, [&](auto a, auto b, auto c) {
//        Q_UNUSED(b);
//        Q_UNUSED(c);
//        QMetaObject::invokeMethod(this, "updateDigitalInputStatus", Qt::QueuedConnection,
//                                  Q_ARG(const DigitalInputStatus&, a));

//    });
    connect(dPtr->ioSignaler.data(), static_cast<void (IOSignaler::*)(const DigitalInputStatus&, const DigitalOutputStatus&, const AnalogInputBufferStatus&)>(&IOSignaler::statusSignal),
            [&](auto a, auto b, auto c) {
        Q_UNUSED(b);
        Q_UNUSED(c);
        QMetaObject::invokeMethod(this, "updateDigitalInputStatus", Qt::QueuedConnection,
                                  Q_ARG(const DigitalInputStatus&, a));

    });


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

    bool checkLimitsAxisX = s.getAxisXCheckLimits();
    bool checkLimitsAxisY = s.getAxisYCheckLimits();
    bool checkLimitsAxisZ = s.getAxisZCheckLimits();

    ui->cbAxisXForwardLimit->setVisible(checkLimitsAxisX);
    ui->cbAxisXReverseLimit->setVisible(checkLimitsAxisX);
    ui->cbAxisYForwardLimit->setVisible(checkLimitsAxisY);
    ui->cbAxisYReverseLimit->setVisible(checkLimitsAxisY);
    ui->cbAxisZForwardLimit->setVisible(checkLimitsAxisZ);
    ui->cbAxisZReverseLimit->setVisible(checkLimitsAxisZ);

    traceExit;

}

MotionFrame::MotionFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MotionFrame), dPtr(new MotionFrameLogic()),
    isHomingAxes(false) {

    dPtr->qPtr = this;
    dPtr->initialize();
    this->setupUi();
    this->setupSignalsAndSlots();

}

MotionFrame::~MotionFrame() {
    delete dPtr;
    delete ui;
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

    bool needResetAxis = motionBean.getNeedResetAxis();

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

    ui->pbMoveX->setEnabled(!needResetAxis && !motionBean.getAxisXMoveInProgress() && !isHomingAxes);
    ui->pbMoveY->setEnabled(!needResetAxis && !motionBean.getAxisYMoveInProgress() && !isHomingAxes);
    ui->pbMoveZ->setEnabled(!needResetAxis && !motionBean.getAxisZMoveInProgress() && !isHomingAxes);

    ui->pbStopX->setEnabled(!needResetAxis && motionBean.getAxisXMoveInProgress() && !isHomingAxes);
    ui->pbStopY->setEnabled(!needResetAxis && motionBean.getAxisYMoveInProgress() && !isHomingAxes);
    ui->pbStopZ->setEnabled(!needResetAxis && motionBean.getAxisZMoveInProgress() && !isHomingAxes);

    ui->pbResetAxes->setEnabled(
                !motionBean.getAxisXMoveInProgress() &&
                !motionBean.getAxisYMoveInProgress() &&
                !motionBean.getAxisZMoveInProgress());

    traceExit;

}
