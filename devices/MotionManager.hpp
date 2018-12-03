#ifndef MOTIONMANAGER_HPP
#define MOTIONMANAGER_HPP

#include <QObject>
#include <QScopedPointer>

#include <AbstractCN.hpp>

namespace PROGRAM_NAMESPACE {

class MotionManager : public QObject {
    Q_OBJECT

public:
    using Ptr = MotionManager*;
    using ConstPtr = const MotionManager*;

public:
    explicit MotionManager(QObject* parent = nullptr);
    ~MotionManager();

    bool moveX(int posMm);

protected:
    void waitAxisXStop();

    virtual bool moveXImpl(int posMm) = 0;
    virtual bool isMotorXOff() = 0;


signals:
    void powerOffSignal();
    void powerOnSignal();
    void cycleOffSignal();
    void cycleOnSignal();
    void axisXMotorOffSignal();
    void axisYMotorOffSignal();
    void axisZMotorOffSignal();
    void axisXMotionStopSignal();
    void axisYMotionStopSignal();
    void axisZMotionStopSignal();

};

}


#endif // MOTIONMANAGER_HPP
