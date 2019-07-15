#ifndef ABSTRACTCONNECTEDDEVICEINSPECTOR_HPP
#define ABSTRACTCONNECTEDDEVICEINSPECTOR_HPP

#include "AbstractDeviceInspector.hpp"

namespace PROGRAM_NAMESPACE {

class AbstractConnectedDeviceInspector : public AbstractDeviceInspector {
    Q_OBJECT

public:
    using Ptr = AbstractConnectedDeviceInspector*;
    using ConstPtr = const AbstractConnectedDeviceInspector*;

public:
    explicit AbstractConnectedDeviceInspector(QObject *parent = nullptr);

protected:
    virtual bool beforeProcess() override;
    virtual void process() override;
    virtual bool isDeviceConnected() = 0;
    virtual bool connectDevice() = 0;

protected slots:
    virtual void handleDisconnection() = 0;

private slots:
    void manageDisconnectedDevice();

signals:
    void failedToConnectSignal();
    void connectedSignal();
    void disconnectedSignal();

};

}

#endif // ABSTRACTCONNECTEDDEVICEINSPECTOR_HPP
