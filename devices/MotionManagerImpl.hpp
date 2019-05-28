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

    typename PROGRAM_NAMESPACE::AbstractCN<S,E>::Ptr cnImpl() {
        return static_cast<typename PROGRAM_NAMESPACE::AbstractCN<S,E>::Ptr>(cn.data());
    }

    QSharedPointer<PROGRAM_NAMESPACE::AbstractCN<S,E> > cn;

public:

    explicit MotionManagerImpl(const QSharedPointer<T>& dev = nullptr, QObject* parent = nullptr) :
        MotionManager(parent),
        cn(dev) {

        static_assert(isCN<T>::value, "Il device deve essere un CN");

    }

    virtual bool isConnected() override {
        return cn->isConnected();
    }

    virtual bool connect() override {
        return cn->connect();
    }

private:

    bool moveImpl(Axis a, posType pos, spdCNType speed, accCNType acc, accCNType dec) {

        traceEnter;

        if (cn.isNull()) {
            traceErr() << "CN non inizializzato";
            return false;
        }

        E err = cn->moveToPosition(a, pos, speed, acc, dec);

        if (cn->isError(err)) {
            traceErr() << "Errore nell'invio dei comandi di spostamento dell'asse X. Err: " << err;
            traceErr() << "Descrizione errore: " << cn->decodeError(err);
            return false;
        }

        err = cn->startMoveAxis(a);

        if (cn->isError(err)) {
            traceErr() << "Errore nello start movimentazione dell'asse X. Err: " << err;
            traceErr() << "Descrizione errore: " << cn->decodeError(err);
            return false;
        }

        traceExit;

        return true;

    }

    bool isMotorOff(Axis a, bool& res) {

        traceEnter;

        if (cn.isNull()) {
            traceErr() << "CN non inizializzato";
            return false;
        }

        E err = cn->isMotorOff(a, res);

        if (cn->isError(err)) {
            traceErr() << "Errore controllo motor off: " << err;
            traceErr() << "Descrizione errore: " << cn->decodeError(err);
            return false;
        }

        traceExit;

        return true;

    }

    bool stopMotor(Axis a) {

        traceEnter;

        if (cn.isNull()) {
            traceErr() << "CN non inizializzato";
            return false;
        }

        E err = cn->stopAxis(a);

        if (cn->isError(err)) {
            traceErr() << "Errore comando stop motore: " << err;
            traceErr() << "Descrizione errore: " << cn->decodeError(err);
            return false;
        }

        traceExit;

        return true;

    }

    bool homeImpl(Axis a, spdCNType speed, accCNType acc, accCNType dec) {

        traceEnter;

        if (cn.isNull()) {
            traceErr() << "CN non inizializzato";
            return false;
        }

        E err = cn->home(a, speed, acc, dec);

        if (cn->isError(err)) {
            traceErr() << "Errore comando stop motore: " << err;
            traceErr() << "Descrizione errore: " << cn->decodeError(err);
            return false;
        }

        err = cn->startMoveAxis(a);

        if (cn->isError(err)) {
            traceErr() << "Errore nello start movimentazione dell'asse X. Err: " << err;
            traceErr() << "Descrizione errore: " << cn->decodeError(err);
            return false;
        }

        traceExit;

        return true;

    }

protected:

    virtual bool moveXImpl(posType pos, spdCNType speed, accCNType acc, accCNType dec) override {

        traceEnter;
        bool isOk = moveImpl(Axis::X, pos, speed, acc, dec);
        traceExit;
        return isOk;

    }

    virtual bool moveYImpl(posType pos, spdCNType speed, accCNType acc, accCNType dec) override {

        traceEnter;
        bool isOk = moveImpl(Axis::Y, pos, speed, acc, dec);
        traceExit;
        return isOk;

    }

    virtual bool moveZImpl(posType pos, spdCNType speed, accCNType acc, accCNType dec) override {

        traceEnter;
        bool isOk = moveImpl(Axis::Z, pos, speed, acc, dec);
        traceExit;
        return isOk;

    }

    virtual bool isMotorXOff(bool& res) {

        traceEnter;
        bool isOk = isMotorOff(Axis::X, res);
        traceExit;
        return isOk;

    }

    virtual bool isMotorYOff(bool& res) {

        traceEnter;
        bool isOk = isMotorOff(Axis::Y, res);
        traceExit;
        return isOk;

    }

    virtual bool isMotorZOff(bool& res) {

        traceEnter;
        bool isOk = isMotorOff(Axis::Z, res);
        traceExit;
        return isOk;

    }

    virtual bool stopXImpl() override {

        traceEnter;
        bool isOk = stopMotor(Axis::X);
        traceExit;
        return isOk;

    }

    virtual bool stopYImpl() override {

        traceEnter;
        bool isOk = stopMotor(Axis::Y);
        traceExit;
        return isOk;

    }

    virtual bool stopZImpl() override {

        traceEnter;
        bool isOk = stopMotor(Axis::Z);
        traceExit;
        return isOk;

    }

    bool homeXImpl(spdCNType speed, accCNType acc, accCNType dec) override {

        traceEnter;
        bool isOk = homeImpl(Axis::X, speed, acc, dec);
        traceExit;
        return isOk;

    }

    bool homeYImpl(spdCNType speed, accCNType acc, accCNType dec) override {

        traceEnter;
        bool isOk = homeImpl(Axis::Y, speed, acc, dec);
        traceExit;
        return isOk;

    }

    bool homeZImpl(spdCNType speed, accCNType acc, accCNType dec) override {

        traceEnter;
        bool isOk = homeImpl(Axis::Z, speed, acc, dec);
        traceExit;
        return isOk;

    }


};

}

#endif // MOTIONMANAGERIMPL_HPP
