#ifndef MOTIONANALIZERFACTORY_HPP
#define MOTIONANALIZERFACTORY_HPP

#include <type_traits>

#include <QObject>

#include <configure.h>
#include <Types.hpp>
#include <MotionBean.hpp>
#include <ConnectedDeviceInspector.hpp>
#include <AbstractCN.hpp>
#include <MotionAnalizer.hpp>
#include <ErrorHandler.hpp>

#include <galil/GalilCNMotionAnalizer.hpp>


template <typename I>
static constexpr bool checkInspector() {
    static_assert(isConnectedDeviceInspector<I>::value, "Il tipo deve essere un ConnectedDeviceInspector");
    using statusT = isConnectedDeviceInspector<I>::status;
    using typeT = isConnectedDeviceInspector<I>::type;

    static_assert(isCN<typeT>::value, "Il device non e' un inspector di un CN");
    static_assert(hasAnalizerTraits<statusT>::value, "Lo stato deve essere analizzabile da un analizer");
    using analizerT = hasAnalizerTraits<statusT>::analizer;
    return true;
}

namespace PROGRAM_NAMESPACE {

class MotionAnalizerFactory {

public:

    template <typename I, typename R = std::enable_if_t<
                  checkInspector<I>(),
                  hasAnalizerTraits<typename isConnectedDeviceInspector<I>::status>::analizer>
              >
    static auto createAnalizer(const QSharedPointer<I>& inspector) -> R* {

        traceEnter;

        // OLD
        // using statusT = isConnectedDeviceInspector<I>::status;
        // using analizerT = hasAnalizerTraits<statusT>::analizer;
        //
        // analizerT* analizer = new analizerT();

        // Nic 2020/01/22 - versione piu compatta, stessa logica scritta sopra
        auto analizer = new R();

        QObject::connect(inspector.data(), &I::statusSignal, [analizer](const QVariant& status) {
            QMetaObject::invokeMethod(analizer, "analize", Qt::QueuedConnection, Q_ARG(QVariant, status));
        });

        traceExit;

        return analizer;

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

};

}

#endif // MOTIONANALIZERFACTORY_HPP
