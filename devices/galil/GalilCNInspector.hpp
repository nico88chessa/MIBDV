#ifndef GALILCNINSPECTOR_HPP
#define GALILCNINSPECTOR_HPP

#include "ConnectedDeviceInspector.hpp"
#include "GalilCNController.hpp"

#include <ErrorHandler.hpp>


namespace PROGRAM_NAMESPACE {

class GalilCNInspector : public ConnectedDeviceInspector<GalilCNController> {
public:
    using Ptr = GalilCNInspector*;
    using ConstPtr = const GalilCNInspector*;

private:
//    GalilCNStatusBean lastStatus;
//    bool isFirst;
//    char isFECheck;
//    bool isCustomHomeAxisX;

//    static constexpr char CUSTOM_HOME_AXIS_X_FE_COUNT = 2;
    DECL_ERROR_SIGNALER_FRIENDS(GalilCNInspector)

    inline GalilCNController::Ptr getGalilCNDevicePtr() {
        return static_cast<GalilCNController::Ptr>(device.data());
    }

public:
    explicit GalilCNInspector(QObject* parent = nullptr);

    ~GalilCNInspector() { }

protected:
    bool connectDevice();

//    void analizeLastStatus(const S& status);

protected slots:

    void handleDisconnection();

};

}

#endif // GALILCNINSPECTOR_HPP
