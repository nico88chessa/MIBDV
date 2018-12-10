#ifndef MOTIONINSPECTOR_HPP
#define MOTIONINSPECTOR_HPP

#include <QObject>

class MotionInspector : public QObject {
    Q_OBJECT

public:
    using Ptr = MotionInspector*;
    using ConstPtr = const MotionInspector*;

public:
    explicit MotionInspector(QObject* parent = nullptr) : QObject(parent) { }
    ~MotionInspector() { }

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

};

#endif // MOTIONINSPECTOR_HPP
