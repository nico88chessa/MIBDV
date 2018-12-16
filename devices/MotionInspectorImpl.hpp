#ifndef MOTIONINSPECTORIMPL_HPP
#define MOTIONINSPECTORIMPL_HPP

#include "AbstractMotionInspector.hpp"
#include "AbstractCN.hpp"
#include <QSharedPointer>

namespace PROGRAM_NAMESPACE {

template <typename T>
class MotionInspectorImpl : public AbstractMotionInspector {
public:
    using Ptr = MotionInspectorImpl*;
    using ConstPtr = const MotionInspectorImpl*;

protected:
    using S = typename isCN<T>::statusType;
    using E = typename isCN<T>::errorType;
    using CN = typename PROGRAM_NAMESPACE::AbstractCN<S,E>;
    using CNPtr = typename PROGRAM_NAMESPACE::AbstractCN<S,E>::Ptr;

    inline CNPtr getDevicePtr() {
        return static_cast<CNPtr>(device.data());
    }

    QScopedPointer<CN> device;

public:
    MotionInspectorImpl(QObject* parent = nullptr) :
        AbstractMotionInspector(parent) {

        static_assert(isCN<T>::value, "Il device non e' un CN");

    }

    ~MotionInspectorImpl() { }

protected:

    void setDevice(CN* dev) {
        device.reset(dev);
    }

    virtual void analizeLastStatus(const S& status) = 0;

    bool getStatus(QVariant& status) {
        traceEnter;
        S s;
        bool res;
        try {
            s = getDevicePtr()->getStatus();
            analizeLastStatus(s);
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

    bool isDeviceConnected() {
        traceEnter;
        bool isConnected = getDevicePtr()->isConnected();
        traceExit;
        return isConnected;
    }

};

}

#endif // MOTIONINSPECTORIMPL_HPP
