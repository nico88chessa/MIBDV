#ifndef MOTIONMANAGERIMPL_HPP
#define MOTIONMANAGERIMPL_HPP

#include <QtGlobal>

#include <Types.hpp>
#include <MotionManager.hpp>
#include <Settings.hpp>
#include <Logger.hpp>
#include <AbstractCN.hpp>

namespace PROGRAM_NAMESPACE {

template <typename T>
class MotionManagerImpl : public MotionManager {
public:
    using Ptr = MotionManagerImpl*;
    using ConstPtr = const MotionManagerImpl*;

private:
    using S = typename PROGRAM_NAMESPACE::isCN<T>::statusType;
    using E = typename PROGRAM_NAMESPACE::isCN<T>::errorType;
//    using ACN = typename PROGRAM_NAMESPACE::AbstractCN<S,E>;

    constexpr typename PROGRAM_NAMESPACE::AbstractCN<S,E>::Ptr cnImpl() {
        return static_cast<typename PROGRAM_NAMESPACE::AbstractCN<S,E>::Ptr>(cn.data());
    }

    QSharedPointer<PROGRAM_NAMESPACE::AbstractCN<S,E> > cn;

public:

    explicit MotionManagerImpl(const QSharedPointer<T>& dev = nullptr, QObject* parent = nullptr) :
        MotionManager(parent),
        cn(dev) {

        static_assert(isCN<T>::value, "Il device deve essere un CN");

    }

protected:

    virtual bool moveXImpl(posType posMm) {

        traceEnter;

        if (cn.isNull()) {
            traceErr() << "CN non inizializzato";
            return false;
        }

        Settings& s = Settings::instance();

        int resolution = s.getAxisXStepPerMm();
        posCNType pos = qRound(posMm * resolution);
        spdCNType speed = s.getAxisXOperativeSpeedMms() * resolution;
        accCNType acc = s.getAxisXOperativeAccMms2() * resolution;
        accCNType dec = s.getAxisXOperativeDecMms2() * resolution;

        E err = cn->moveToPosition(Axis::X, pos, speed, acc, dec);

        if (cn->isError(err)) {
            traceErr() << "Errore nell'invio dei comandi di spostamento dell'asse X. Err: " << err;
            traceErr() << "Descrizione errore: " << cn->decodeError(err);
            return false;
        }

        err = cn->startMoveAxis(Axis::X);

        if (cn->isError(err)) {
            traceErr() << "Errore nello start movimentazione dell'asse X. Err: " << err;
            traceErr() << "Descrizione errore: " << cn->decodeError(err);
            return false;
        }

        traceExit;

        return true;

    }

    bool isMotorXOff(bool& res) {

        traceEnter;

        if (cn.isNull()) {
            traceErr() << "CN non inizializzato";
            return false;
        }

        E err = cn->isMotorOff(Axis::X, res);

        if (cn->isError(err)) {
            traceErr() << "Errore check motor off: " << err;
            traceErr() << "Descrizione errore: " << cn->decodeError(err);
            return false;
        }

        traceExit;

        return true;

    }

};

}

#endif // MOTIONMANAGERIMPL_HPP
