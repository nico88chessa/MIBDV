#ifndef MOTIONMANAGERIMPL_HPP
#define MOTIONMANAGERIMPL_HPP

#include <CommonTraits.hpp>
#include <MotionManager.hpp>
#include <AbstractCN.hpp>
#include <Settings.hpp>
#include <Logger.hpp>

namespace PROGRAM_NAMESPACE {

template <typename T>
class MotionManagerImpl : public MotionManager {
public:
    using Ptr = MotionManagerImpl*;
    using ConstPtr = const MotionManagerImpl*;

private:
    using S = typename PROGRAM_NAMESPACE::isCN<T>::statusType;
    using E = typename PROGRAM_NAMESPACE::isCN<T>::errorType;
    using ACN = typename PROGRAM_NAMESPACE::AbstractCN<S,E>;

    constexpr typename PROGRAM_NAMESPACE::AbstractCN<S,E>::Ptr cnImpl() {
        return static_cast<typename PROGRAM_NAMESPACE::AbstractCN<S,E>::Ptr>(cn.data());
    }

    QSharedPointer<PROGRAM_NAMESPACE::AbstractCN<S,E> > cn;

public:

    explicit MotionManagerImpl(const QSharedPointer<T>& dev = nullptr, QObject* parent = nullptr) :
        MotionManager(parent),
        cn(dev) {

        using namespace PROGRAM_NAMESPACE;
        static_assert(isCN<T>::value, "Il device deve essere un CN");

    }

protected:

    virtual bool moveXImpl(int posMm) {

        using namespace PROGRAM_NAMESPACE;

        traceEnter;

        if (cn.isNull())
            return false;

        cn->setDigitalOutput(0, true);

        Settings& s = Settings::instance();

        int resolution = s.getAxisXStepPerMm();
        typename ACN::posType pos = posMm * resolution;
        typename ACN::spdType speed = s.getAxisXOperativeSpeedMms() * resolution;
        typename ACN::accType acc = s.getAxisXOperativeAccMms2() * resolution;
        typename ACN::accType dec = s.getAxisXOperativeDecMms2() * resolution;

        E err = cn->moveToPosition(Axis::X, pos, speed, acc, dec);

        if (cn->isError(err)) {
            traceErr() << "Errore nello spostamento dell'asse X; err: " << err;
            return false;
        }

        traceExit;

        return true;

    }

    bool isMotorXOff() {

        traceEnter;
        using namespace PROGRAM_NAMESPACE;

        if (cn.isNull())
            return false;

        bool isMotorOff;
        E err = cn->isMotorOff(Axis::X, isMotorOff);

        if (cn->isError(err)) {
            traceErr() << "Errore check motor off: " << err;
            return  false;
        }

        traceExit;

        return isMotorOff;

    }

};

}

#endif // MOTIONMANAGERIMPL_HPP
