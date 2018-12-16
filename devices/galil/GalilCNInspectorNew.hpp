#ifndef GALILCNINSPECTORNEW_HPP
#define GALILCNINSPECTORNEW_HPP

#include "MotionInspectorImpl.hpp"
#include "GalilCNController.hpp"

namespace PROGRAM_NAMESPACE {

class GalilCNInspectorNew : public MotionInspectorImpl<GalilCNController> {
public:
    using Ptr = GalilCNInspectorNew*;
    using ConstPtr = const GalilCNInspectorNew*;

private:
    GalilCNStatusBean lastStatus;
    QString ipAddress;
    bool isFirst;

    inline GalilCNController::Ptr getGalilCNDevicePtr() {
        return static_cast<GalilCNController::Ptr>(device.data());
    }

public:
    explicit GalilCNInspectorNew(QObject* parent = nullptr);

    ~GalilCNInspectorNew() { }

protected:
    bool connectDevice();

    void analizeLastStatus(const S& status);

};

}

#endif // GALILCNINSPECTORNEW_HPP
