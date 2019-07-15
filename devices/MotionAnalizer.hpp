#ifndef MOTIONANALIZER_HPP
#define MOTIONANALIZER_HPP

#include <QObject>

#include <configure.h>

#include <Types.hpp>
#include <MotionBean.hpp>

#include <ErrorHandler.hpp>
#include <MachineStatusHandler.hpp>


namespace PROGRAM_NAMESPACE {

class IMotionAnalizer : public QObject {
    Q_OBJECT

public:
    using Ptr = IMotionAnalizer *;
    using ConstPtr = const IMotionAnalizer *;

public:
    explicit IMotionAnalizer(QObject* parent = nullptr) :
        QObject(parent) {

        initializeErrorSignaler(this->errorSignaler, this);
        initializeMachineStatusReceiver(this->machineStatusR, this);

    }

protected:
    virtual void analizeImpl(const QVariant& status) = 0;
    DECL_ERROR_SIGNALER_FRIENDS(IMotionAnalizer)
    DECL_MACHINE_STATUS_RECEIVER(IMotionAnalizer)

public slots:
    void analize(const QVariant& status) {
        analizeImpl(status);
    }

signals:
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

    void motionBeanSignal(const MotionBean& motionBean);

};


template <typename T>
class AbstractAnalizer : public IMotionAnalizer {
public:
    using Ptr = AbstractAnalizer*;
    using ConstPtr = const AbstractAnalizer*;

protected:
    virtual void analizeImpl(const T& newStatus) = 0;

public:
    AbstractAnalizer(QObject* parent = nullptr) :
        IMotionAnalizer(parent) { }

    virtual void analizeImpl(const QVariant& status) override final {
        auto obj = qvariant_cast<T>(status);
        analizeImpl(obj);
        emit motionBeanSignal(MotionBean(obj));
    }

};


// TRAITS

template <typename T>
struct hasAnalizerTraits {
    static constexpr bool value = false;
    using analizer = void;
};

}

#endif // MOTIONANALIZER_HPP
