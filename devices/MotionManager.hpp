#ifndef MOTIONMANAGER_HPP
#define MOTIONMANAGER_HPP

#include <QtCore>
#include <QObject>
#include <QScopedPointer>

#include <Constants.hpp>

namespace PROGRAM_NAMESPACE {

using MotionErr = int;

static constexpr MotionErr MOTION_MANAGER_NO_ERR = PROGRAM_ERR_START_CODE + 1;
static constexpr MotionErr MOTION_MANAGER_ERR_CN_PROBLEM = PROGRAM_ERR_START_CODE + 2;
static constexpr MotionErr MOTION_MANAGER_MOTOR_X_OFF = PROGRAM_ERR_START_CODE + 3;
static constexpr MotionErr MOTION_MANAGER_MOTOR_Y_OFF = PROGRAM_ERR_START_CODE + 4;
static constexpr MotionErr MOTION_MANAGER_MOTOR_Z_OFF = PROGRAM_ERR_START_CODE + 5;
static constexpr MotionErr MOTION_MANAGER_TIMEOUT = PROGRAM_ERR_START_CODE + 6;
static constexpr MotionErr MOTION_MANAGER_CYCLE_OFF = PROGRAM_ERR_START_CODE + 7;
static constexpr MotionErr MOTION_MANAGER_POWER_OFF = PROGRAM_ERR_START_CODE + 8;
static constexpr MotionErr MOTION_MANAGER_MOTION_X_STOP_CORRECTLY = PROGRAM_ERR_START_CODE + 9;
static constexpr MotionErr MOTION_MANAGER_MOTION_Y_STOP_CORRECTLY = PROGRAM_ERR_START_CODE + 10;
static constexpr MotionErr MOTION_MANAGER_MOTION_Z_STOP_CORRECTLY = PROGRAM_ERR_START_CODE + 11;
static constexpr MotionErr MOTION_MANAGER_MOTION_X_STOP_ERROR = PROGRAM_ERR_START_CODE + 12;
static constexpr MotionErr MOTION_MANAGER_MOTION_Y_STOP_ERROR = PROGRAM_ERR_START_CODE + 13;
static constexpr MotionErr MOTION_MANAGER_MOTION_Z_STOP_ERROR = PROGRAM_ERR_START_CODE + 14;
static constexpr MotionErr MOTION_MANAGER_STOP_COMMAND_X_ERR = PROGRAM_ERR_START_CODE + 15;
static constexpr MotionErr MOTION_MANAGER_STOP_COMMAND_Y_ERR = PROGRAM_ERR_START_CODE + 16;
static constexpr MotionErr MOTION_MANAGER_STOP_COMMAND_Z_ERR = PROGRAM_ERR_START_CODE + 17;
static constexpr MotionErr MOTION_MANAGER_HOME_X_COMPLETED_CORRECTLY = PROGRAM_ERR_START_CODE + 18;
static constexpr MotionErr MOTION_MANAGER_HOME_Y_COMPLETED_CORRECTLY = PROGRAM_ERR_START_CODE + 19;
static constexpr MotionErr MOTION_MANAGER_HOME_Z_COMPLETED_CORRECTLY = PROGRAM_ERR_START_CODE + 20;
static constexpr MotionErr MOTION_MANAGER_RESET_NOTIFY_KO = PROGRAM_ERR_START_CODE + 21;


class MotionManager : public QObject {
    Q_OBJECT

public:
    using Ptr = MotionManager*;
    using ConstPtr = const MotionManager*;

public:
    explicit MotionManager(QObject* parent = nullptr);
    ~MotionManager();

    virtual bool isConnected() = 0;
    virtual bool connect() = 0;

    MotionErr moveX(posType posMm);
    MotionErr moveXManual(posType posMm);
    MotionErr stopX();
    MotionErr homeX();

    MotionErr moveY(posType posMm);
    MotionErr moveYManual(posType posMm);
    MotionErr stopY();
    MotionErr homeY();

    MotionErr moveZ(posType posMm);
    MotionErr moveZManual(posType posMm);
    MotionErr stopZ();
    MotionErr homeZ();

    MotionErr notifyResetOk();

protected:
    virtual bool moveXImpl(posType posMm, spdCNType speed, accCNType acc, accCNType dec) = 0;
    virtual bool moveYImpl(posType posMm, spdCNType speed, accCNType acc, accCNType dec) = 0;
    virtual bool moveZImpl(posType posMm, spdCNType speed, accCNType acc, accCNType dec) = 0;

    virtual bool stopXImpl() = 0;
    virtual bool stopYImpl() = 0;
    virtual bool stopZImpl() = 0;

    virtual bool isMotorXOff(bool& res) = 0;
    virtual bool isMotorYOff(bool& res) = 0;
    virtual bool isMotorZOff(bool& res) = 0;

    virtual bool homeXImpl(spdCNType speed, accCNType acc, accCNType dec) = 0;
    virtual bool homeYImpl(spdCNType speed, accCNType acc, accCNType dec) = 0;
    virtual bool homeZImpl(spdCNType speed, accCNType acc, accCNType dec) = 0;

    virtual bool notifyResetOkImpl() = 0;

signals:
    //void powerOffSignal();
    //void powerOnSignal();
    //void cycleOffSignal();
    //void cycleOnSignal();

    void axisXMotorOffSignal();
    void axisXMotionStopSignal(MotionStopCode stopCode);
    void axisXForwardLimitSignal();
    void axisXBackwardLimitSignal();
    void axisXHomeInProgressStartSignal();
    void axisXHomeInProgressStopSignal();
    void axisXHomingComplete();

    void axisYMotorOffSignal();
    void axisYMotionStopSignal(MotionStopCode stopCode);
    void axisYForwardLimitSignal();
    void axisYBackwardLimitSignal();
    void axisYHomeInProgressStartSignal();
    void axisYHomeInProgressStopSignal();
    void axisYHomingComplete();

    void axisZMotorOffSignal();
    void axisZMotionStopSignal(MotionStopCode stopCode);
    void axisZForwardLimitSignal();
    void axisZBackwardLimitSignal();
    void axisZHomeInProgressStartSignal();
    void axisZHomeInProgressStopSignal();
    void axisZHomingComplete();

public:

    static QString decodeError(MotionErr error) {

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
            case MOTION_MANAGER_MOTION_Y_STOP_CORRECTLY: errDescr = tr("Spostamento asse Y completato correttamente"); break;
            case MOTION_MANAGER_MOTION_Z_STOP_CORRECTLY: errDescr = tr("Spostamento asse Z completato correttamente"); break;
            case MOTION_MANAGER_MOTION_X_STOP_ERROR: errDescr = tr("Movimento asse X interrotto"); break;
            case MOTION_MANAGER_MOTION_Y_STOP_ERROR: errDescr = tr("Movimento asse Y interrotto"); break;
            case MOTION_MANAGER_MOTION_Z_STOP_ERROR: errDescr = tr("Movimento asse Z interrotto"); break;
            case MOTION_MANAGER_STOP_COMMAND_X_ERR: errDescr = tr("Errore nel comando di stop per l'asse X"); break;
            case MOTION_MANAGER_STOP_COMMAND_Y_ERR: errDescr = tr("Errore nel comando di stop per l'asse Y"); break;
            case MOTION_MANAGER_STOP_COMMAND_Z_ERR: errDescr = tr("Errore nel comando di stop per l'asse Z"); break;
            case MOTION_MANAGER_HOME_X_COMPLETED_CORRECTLY: errDescr = tr("Homing asse X completato correttamente"); break;
            case MOTION_MANAGER_HOME_Y_COMPLETED_CORRECTLY: errDescr = tr("Homing asse Y completato correttamente"); break;
            case MOTION_MANAGER_HOME_Z_COMPLETED_CORRECTLY: errDescr = tr("Homing asse Z completato correttamente"); break;
            case MOTION_MANAGER_RESET_NOTIFY_KO: errDescr = tr("Failed to notify reset ok to device"); break;
            default: ;
        }
        return errDescr;

    }

    inline bool isErr(MotionErr error) {

        bool isErr = (error != MOTION_MANAGER_NO_ERR) &&
                     (error != MOTION_MANAGER_MOTION_X_STOP_CORRECTLY) &&
                     (error != MOTION_MANAGER_MOTION_Y_STOP_CORRECTLY) &&
                     (error != MOTION_MANAGER_MOTION_Z_STOP_CORRECTLY) &&
                     (error != MOTION_MANAGER_HOME_X_COMPLETED_CORRECTLY) &&
                     (error != MOTION_MANAGER_HOME_Y_COMPLETED_CORRECTLY) &&
                     (error != MOTION_MANAGER_HOME_Z_COMPLETED_CORRECTLY);

        return isErr;

    }

};

}


#endif // MOTIONMANAGER_HPP
