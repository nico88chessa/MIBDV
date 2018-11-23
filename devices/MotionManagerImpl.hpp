#ifndef MOTIONMANAGERIMPL_HPP
#define MOTIONMANAGERIMPL_HPP

#include <CommonTraits.hpp>
#include <MotionManager.hpp>
#include <AbstractCN.hpp>
#include <Settings.hpp>
#include <Logger.hpp>

template <typename T>
class MotionManagerImpl : public MotionManager {
public:
    using Ptr = MotionManagerImpl*;
    using ConstPtr = const MotionManagerImpl*;

private:
    using S = typename PROGRAM_NAMESPACE::isCN<T>::statusType;
    using E = typename PROGRAM_NAMESPACE::isCN<T>::errorType;

    constexpr typename PROGRAM_NAMESPACE::AbstractCN<S,E>::Ptr cnImpl() {
        return static_cast<typename PROGRAM_NAMESPACE::AbstractCN<S,E>::Ptr>(cn.data());
    }

    QSharedPointer<PROGRAM_NAMESPACE::AbstractCN<S,E> > cn;

public:

    explicit MotionManagerImpl(QObject* parent = nullptr, const QSharedPointer<T>& dev = nullptr) :
        MotionManager(parent),
        cn(dev) {

        using namespace PROGRAM_NAMESPACE;
        static_assert(isCN<T>::value, "Il device deve essere un CN");
        if (!cn.isNull())
            this->initCN();

    }

protected:

    virtual bool moveXImpl(int posMm) {

        using namespace PROGRAM_NAMESPACE;

        traceEnter;

        if (cn.isNull())
            return false;

        cn->setDigitalOutput(0, true);

        traceExit;

        return true;

    }

    virtual void initCN() {
        using namespace PROGRAM_NAMESPACE;

        cn->setupController(8,8,0);
        cn->connect(Settings::instance().getGalilCNIpAddress());

    }
};

#endif // MOTIONMANAGERIMPL_HPP
