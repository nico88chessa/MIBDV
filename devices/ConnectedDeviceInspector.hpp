#ifndef CONNECTEDDEVICEINSPECTOR_HPP
#define CONNECTEDDEVICEINSPECTOR_HPP

#include <type_traits>

#include <QScopedPointer>

#include "AbstractConnectedDeviceInspector.hpp"
#include <configure.h>
#include <AbstractCN.hpp>
#include <AbstractPLC.hpp>


namespace PROGRAM_NAMESPACE {

template <typename T>
class ConnectedDeviceInspector : public AbstractConnectedDeviceInspector {
public:
    using Ptr = ConnectedDeviceInspector*;
    using ConstPtr = const ConnectedDeviceInspector*;
    using type = T;
    using statusType = typename isDevice<T>::statusType;
    using Dev = typename AbstractDevice<statusType>;
    using DevPtr = typename Dev::Ptr;

protected:
    inline DevPtr getDevicePtr() {
        return static_cast<DevPtr>(device.data());
    }

public:
    QScopedPointer<Dev> device;

public:

    ConnectedDeviceInspector(QObject* parent = nullptr) :
        AbstractConnectedDeviceInspector(parent) {
        static_assert(isDevice<type>::value, "Il tipo deve essere un device");
    }

    ~ConnectedDeviceInspector() { }

protected:
    void setDevice(DevPtr dev) {
        device.reset(dev);
    }

    virtual bool getStatus(QVariant& status) override {

        traceEnter;
        bool res;
        try {
            statusType s = getDevicePtr()->getStatus();
            status = QVariant::fromValue(s);
            res = true;
        } catch (NoStatusException& e) {
            Q_UNUSED(e)
            res = false;
            traceErr() << "Impossibile avere lo stato del device";
        }
        traceExit;
        return res;

    }

    virtual bool isDeviceConnected() override {
        traceEnter;
        bool isConnected = device->isConnected();
        traceExit;
        return isConnected;
    }

    bool connectDevice() {
        return true;
    }

protected slots:
    void handleDisconnection() { }
};


template <typename T, typename = void>
struct isConnectedDeviceInspector {
    static constexpr bool value = false;
};

template <typename T>
struct isConnectedDeviceInspector<
        T,
        typename std::enable_if_t<
            std::is_base_of<ConnectedDeviceInspector<typename T::type>, T>::value>
        > {
    static constexpr bool value = true;
    using type = typename T::type;
    using status = typename T::statusType;
};

template <typename T, typename = void>
struct isMotionInspector {
    static constexpr bool value = false;
};

template <typename T>
struct isMotionInspector<T, typename std::enable_if_t<isCN<typename T::type>::value> > {
    static constexpr bool value = true;
    using status = typename isCN<typename T::type>::statusType;
};

template <typename T>
static constexpr bool testMotionIns() {
    return isMotionInspector<T>::value;
}

/* NOTE NIC 23/05/2019 - decommentare se potrebbero essere utili

template <typename T,
        typename std::enable_if_t<!isConnectedDeviceInspector<T>::value>* = nullptr>
static constexpr bool isCNInspector() {
    return false;
}

template <typename T,
        typename std::enable_if_t<isConnectedDeviceInspector<T>::value>* = nullptr>
static constexpr bool isCNInspector() {
    return isCN<typename T::type>::value;
}

template <typename T,
        typename std::enable_if_t<!isConnectedDeviceInspector<T>::value>* = nullptr>
static constexpr bool isPLCInspector() {
    return false;
}

template <typename T,
        typename std::enable_if_t<isConnectedDeviceInspector<T>::value>* = nullptr>
static constexpr bool isPLCInspector() {
    return isPLC<typename T::type>::value;
}

*/

}

#endif // CONNECTEDDEVICEINSPECTOR_HPP
