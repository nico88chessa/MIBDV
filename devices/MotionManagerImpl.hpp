#ifndef MOTIONMANAGERIMPL_HPP
#define MOTIONMANAGERIMPL_HPP

#include <CommonTraits.hpp>
#include <MotionManager.hpp>
#include <GalilCNController.hpp>
#include <Settings.hpp>

template <PROGRAM_NAMESPACE::DeviceKey D>
class MotionManagerImpl : public MotionManager {
public:
    using Ptr = MotionManagerImpl*;
    using ConstPtr = const MotionManagerImpl*;

private:
    using C = typename PROGRAM_NAMESPACE::deviceKeyTraits<D>::type;
    using S = typename PROGRAM_NAMESPACE::isCN<C>::statusType;
    using E = typename PROGRAM_NAMESPACE::isCN<C>::errorType;

    constexpr typename PROGRAM_NAMESPACE::AbstractCN<S,E>::Ptr cnImpl() {
        return static_cast<typename PROGRAM_NAMESPACE::AbstractCN<S,E>::Ptr>(cn.data());
    }

public:
    explicit MotionManagerImpl(QObject* parent = nullptr) : MotionManager(parent) {

        using namespace PROGRAM_NAMESPACE;

        using deviceType = typename deviceKeyTraits<D>::type;
        static_assert(isCN<deviceType>::value, "Il device deve essere un CN");

        this->initCN();

    }

protected:

    int moveXImpl(int posMm) {
        cnImpl()->setDigitalOutput(0, true);
        return 0;

    }

    virtual void initCN() {
        using namespace PROGRAM_NAMESPACE;

        switch (D) {
            case DeviceKey::GALIL_CN:
                GalilCNController::Ptr galilCN = new GalilCNController();
                galilCN->setupController(8,8,0);
                galilCN->connect(Settings::instance().getGalilCNIpAddress());
                cn.reset(galilCN);
                break;
        }

    }
};

#endif // MOTIONMANAGERIMPL_HPP
