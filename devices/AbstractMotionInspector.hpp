#ifndef ABSTRACTMOTIONINSPECTOR_HPP
#define ABSTRACTMOTIONINSPECTOR_HPP

#include "AbstractConnectedDeviceInspector.hpp"
#include "AbstractCN.hpp"

namespace PROGRAM_NAMESPACE {

class AbstractMotionInspector : public AbstractConnectedDeviceInspector {
    Q_OBJECT

public:
    using Ptr = AbstractMotionInspector*;
    using ConstPtr = const AbstractMotionInspector*;

public:
    explicit AbstractMotionInspector(QObject* parent = nullptr) :
        AbstractConnectedDeviceInspector(parent) { }

    ~AbstractMotionInspector() { }

signals:
    void axisXMotorOffSignal();
    void axisXMotionStopSignal(MotionStopCode stopCode);
    void axisXForwardLimitSignal();
    void axisXBackwardLimitSignal();
    void axisXHomeInProgressStartSignal();
    void axisXHomeInProgressStopSignal();

    void axisYMotorOffSignal();
    void axisYMotionStopSignal(MotionStopCode stopCode);
    void axisYForwardLimitSignal();
    void axisYBackwardLimitSignal();
    void axisYHomeInProgressStartSignal();
    void axisYHomeInProgressStopSignal();

    void axisZMotorOffSignal();
    void axisZMotionStopSignal(MotionStopCode stopCode);
    void axisZForwardLimitSignal();
    void axisZBackwardLimitSignal();
    void axisZHomeInProgressStartSignal();
    void axisZHomeInProgressStopSignal();

};

}

#endif // ABSTRACTMOTIONINSPECTOR_HPP
