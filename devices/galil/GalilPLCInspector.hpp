#ifndef GALILPLCINSPECTOR_HPP
#define GALILPLCINSPECTOR_HPP

#include "ConnectedDeviceInspector.hpp"
#include "GalilPLCController.hpp"

#include <ErrorHandler.hpp>


namespace PROGRAM_NAMESPACE {

class GalilPLCInspector : public ConnectedDeviceInspector<GalilPLCController> {
    Q_OBJECT

public:
    using Ptr = GalilPLCInspector*;
    using ConstPtr = const GalilPLCInspector*;

private:
    GalilPLCStatusBean lastStatus;
    DECL_ERROR_SIGNALER_FRIENDS(GalilPLCInspector)

    inline GalilPLCController::Ptr getGalilPLCDevicePtr() {
        return static_cast<GalilPLCController::Ptr>(device.data());
    }

public:
    explicit GalilPLCInspector(QObject* parent = nullptr);

    ~GalilPLCInspector() { }

protected:

//    bool getStatus(QVariant& status);

    bool isDeviceConnected();

    bool connectDevice();

protected slots:
    void handleDisconnection();
};

}

#endif // GALILPLCINSPECTOR_HPP
