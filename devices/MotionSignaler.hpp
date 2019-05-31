#ifndef MOTIONSIGNALER_HPP
#define MOTIONSIGNALER_HPP

#include <type_traits>

#include <QObject>

#include <configure.h>
#include <Types.hpp>
#include <MotionBean.hpp>
#include <ConnectedDeviceInspector.hpp>
#include <AbstractCN.hpp>
#include <MotionAnalizer.hpp>
#include <ErrorManager.hpp>

#include <galil/GalilCNMotionAnalizer.hpp>


namespace PROGRAM_NAMESPACE {

class MotionSignaler : public QObject {
    Q_OBJECT

public:
    using Ptr = MotionSignaler*;
    using ConstPtr = const MotionSignaler*;

    // qui ci va la lista di amici (tutti gli analizer)
    friend class GalilCNMotionAnalizer;

private:
    IMotionAnalizer* analizer;
    DECL_ERROR_SIGNALER_FRIENDS(MotionSignaler)

public:

    explicit MotionSignaler(QObject* parent = nullptr) : QObject(parent), errorSignaler(new ErrorSignaler()) { }
    ~MotionSignaler() { }

    template <typename I>
    void subscribeInspector(const QSharedPointer<I>& inspector) {

        traceEnter;

        static_assert(isConnectedDeviceInspector<I>::value, "Il tipo deve essere un ConnectedDeviceInspector");
        using statusT = isConnectedDeviceInspector<I>::status;
        using typeT = isConnectedDeviceInspector<I>::type;

        static_assert(isCN<typeT>::value, "Il device non e' un inspector di un CN");
        static_assert(hasAnalizerTraits<statusT>::value, "Lo stato deve essere analizzabile da un analizer");
        using analizerT = hasAnalizerTraits<statusT>::analizer;

        analizer = new analizerT(this);
        analizer->setSignaler(this);
        connect(inspector.data(), &I::statusSignal, [&](const QVariant& status) {
            QMetaObject::invokeMethod(analizer, "analize", Qt::QueuedConnection, Q_ARG(QVariant, status));
        });

        traceExit;

        /* errato - la chiamata viene fatta nel thread che genera il signal
        connect(inspector.data(), &I::statusSignal, [&](const QVariant& status) {
            this->analizeStatus(qvariant_cast<typename isMotionInspector<I>::status>(status));
        });
        */

    }

//    template <typename I,
//              typename std::enable_if_t<isConnectedDeviceInspector<I>::value>* = nullptr>
//    void subscribeInspector(const QSharedPointer<I>& inspector) {

//        //static_assert(isConnectedDeviceInspector<I>::value, "Il tipo deve essere un ConnectedDeviceInspector");
//        using statusT = isConnectedDeviceInspector<I>::status;
//        using typeT = isConnectedDeviceInspector<I>::type;

//        static_assert(isCN<typeT>::value, "Il device non e' un inspector di un CN");

//        if (std::is_same<GalilCNController, typeT>::value) {
//            connect(inspector.data(), &I::statusSignal, [&](const QVariant& status) {
//                auto p = qvariant_cast<statusT>(status);
//                QMetaObject::invokeMethod(this, "analizeStatus", Qt::QueuedConnection,
//                                          Q_ARG(const GalilCNStatusBean&, p));
//            });
//        }

//        /* errato - la chiamata viene fatta nel thread che genera il signal
//        connect(inspector.data(), &I::statusSignal, [&](const QVariant& status) {
//            this->analizeStatus(qvariant_cast<typename isMotionInspector<I>::status>(status));
//        });
//        */

//    }


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

}

#endif // MOTIONSIGNALER_HPP
