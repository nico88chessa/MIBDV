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
    void powerOffSignal();
    void powerOnSignal();
    void cycleOffSignal();
    void cycleOnSignal();

    void axisXMotorOffSignal();
    void axisXMotionStopSignal();
    void axisXForwardLimitSignal();
    void axisXBackwardLimitSignal();
    void axisXHomeInProgressStartSignal();
    void axisXHomeInProgressStopSignal();

    void axisYMotorOffSignal();
    void axisYMotionStopSignal();
    void axisYForwardLimitSignal();
    void axisYBackwardLimitSignal();
    void axisYHomeInProgressStartSignal();
    void axisYHomeInProgressStopSignal();

    void axisZMotorOffSignal();
    void axisZMotionStopSignal();
    void axisZForwardLimitSignal();
    void axisZBackwardLimitSignal();
    void axisZHomeInProgressStartSignal();
    void axisZHomeInProgressStopSignal();

protected:
    virtual void handleDisconnetionImpl() {
        // do nothing
    }


protected slots:
    void handleDisconnection() {
        traceEnter;
        handleDisconnetionImpl();
        traceExit;
    }
};

}

#endif // ABSTRACTMOTIONINSPECTOR_HPP
