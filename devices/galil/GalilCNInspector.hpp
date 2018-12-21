#ifndef GALILCNINSPECTOR_HPP
#define GALILCNINSPECTOR_HPP

#include "MotionInspectorImpl.hpp"
#include "GalilCNController.hpp"

namespace PROGRAM_NAMESPACE {

class GalilCNInspector : public MotionInspectorImpl<GalilCNController> {
public:
    using Ptr = GalilCNInspector*;
    using ConstPtr = const GalilCNInspector*;

private:
    GalilCNStatusBean lastStatus;
    QString ipAddress;
    bool isFirst;

    inline GalilCNController::Ptr getGalilCNDevicePtr() {
        return static_cast<GalilCNController::Ptr>(device.data());
    }

public:
    explicit GalilCNInspector(QObject* parent = nullptr);

    ~GalilCNInspector() { }

protected:
    bool connectDevice();

    void analizeLastStatus(const S& status);

};

}

#endif // GALILCNINSPECTOR_HPP
