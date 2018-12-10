#ifndef MOTIONMANAGER_HPP
#define MOTIONMANAGER_HPP

#include <QtCore>
#include <QObject>
#include <QScopedPointer>

#include <Constants.hpp>

namespace PROGRAM_NAMESPACE {

static constexpr int MOTION_MANAGER_NO_ERR = PROGRAM_ERR_START_CODE + 1;
static constexpr int MOTION_MANAGER_ERR_CN_PROBLEM = PROGRAM_ERR_START_CODE + 2;
static constexpr int MOTION_MANAGER_MOTOR_X_OFF = PROGRAM_ERR_START_CODE + 3;
static constexpr int MOTION_MANAGER_MOTOR_Y_OFF = PROGRAM_ERR_START_CODE + 4;
static constexpr int MOTION_MANAGER_MOTOR_Z_OFF = PROGRAM_ERR_START_CODE + 5;
static constexpr int MOTION_MANAGER_TIMEOUT = PROGRAM_ERR_START_CODE + 6;
static constexpr int MOTION_MANAGER_CYCLE_OFF = PROGRAM_ERR_START_CODE + 7;
static constexpr int MOTION_MANAGER_POWER_OFF = PROGRAM_ERR_START_CODE + 8;
static constexpr int MOTION_MANAGER_MOTION_X_STOP_CORRECTLY = PROGRAM_ERR_START_CODE + 9;


class MotionManager : public QObject {
    Q_OBJECT

public:
    using Ptr = MotionManager*;
    using ConstPtr = const MotionManager*;

public:
    explicit MotionManager(QObject* parent = nullptr);
    ~MotionManager();

    int moveX(posType posMm);

protected:
    void waitAxisXStop();

    virtual bool moveXImpl(posType posMm) = 0;
    virtual bool isMotorXOff(bool& res) = 0;

signals:
    void powerOffSignal();
    void powerOnSignal();
    void cycleOffSignal();
    void cycleOnSignal();

    void axisXMotorOffSignal();
    void axisXMotionStopSignal();
    void axisXForwardLimitSignal();
    void axisXBackwardLimitSignal();

    void axisYMotorOffSignal();
    void axisYMotionStopSignal();
    void axisYForwardLimitSignal();
    void axisYBackwardLimitSignal();

    void axisZMotorOffSignal();
    void axisZMotionStopSignal();
    void axisZForwardLimitSignal();
    void axisZBackwardLimitSignal();

public:

    static QString decodeError(int error) {

        QString errDescr;
        switch (error) {
            case MOTION_MANAGER_ERR_CN_PROBLEM: errDescr = tr("Problema comunicazione con il CN"); break;
            case MOTION_MANAGER_MOTOR_X_OFF: errDescr = tr("Asse X non e' in coppia"); break;
            case MOTION_MANAGER_MOTOR_Y_OFF: errDescr = tr("Asse Y non e' in coppia"); break;
            case MOTION_MANAGER_MOTOR_Z_OFF: errDescr = tr("Asse Z non e' in coppia"); break;
            case MOTION_MANAGER_TIMEOUT: errDescr = tr("Timeout nella movimentazione"); break;
            case MOTION_MANAGER_CYCLE_OFF: errDescr = tr("Movimentazione interrotta da cycle off"); break;
            case MOTION_MANAGER_POWER_OFF: errDescr = tr("Movimentazione interrotta da power off"); break;
            case MOTION_MANAGER_MOTION_X_STOP_CORRECTLY: errDescr = tr("Spostamento asse X completato correttamente"); break;
            default: ;
        }
        return errDescr;


    }

    inline bool isErr(int error) {

        bool isErr = (error != MOTION_MANAGER_NO_ERR) &&
                     (error != MOTION_MANAGER_MOTION_X_STOP_CORRECTLY);

        return isErr;

    }

};

}


#endif // MOTIONMANAGER_HPP
