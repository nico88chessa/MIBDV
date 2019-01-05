#ifndef GALILPLCINSPECTOR_HPP
#define GALILPLCINSPECTOR_HPP

#include <devices/AbstractConnectedDeviceInspector.hpp>

#include "GalilPLCController.hpp"

namespace PROGRAM_NAMESPACE {

class GalilPLCInspector : public AbstractConnectedDeviceInspector {
    Q_OBJECT

public:
    using Ptr = GalilPLCInspector*;
    using ConstPtr = const GalilPLCInspector*;

private:
    GalilPLCStatusBean lastStatus;
    QString ipAddress;
    QScopedPointer<GalilPLCController> device;

    inline GalilPLCController::Ptr getGalilPLCDevicePtr() {
        return static_cast<GalilPLCController::Ptr>(device.data());
    }

public:
    explicit GalilPLCInspector(QObject* parent = nullptr);

protected:

    bool getStatus(QVariant& status);

    bool isDeviceConnected();

    bool connectDevice();

protected slots:
    void handleDisconnection();
};

}

#endif // GALILPLCINSPECTOR_HPP
