#ifndef ABSTRACTDEVICECONNECTIONWATCHER_HPP
#define ABSTRACTDEVICECONNECTIONWATCHER_HPP

#include <QObject>
#include <QSharedPointer>
#include <QTimer>
#include <QThread>

#include <Types.hpp>

#include <devices/AbstractDevice.hpp>

namespace PROGRAM_NAMESPACE {

class AbstractDeviceConnectionWatcher : public QObject {
    Q_OBJECT

public:
    using Ptr = AbstractDeviceConnectionWatcher*;
    using ConstPtr = const AbstractDeviceConnectionWatcher*;

protected:
    QSharedPointer<IAbstractDevice> device;
    QTimer connectionCheckTimer;
    DeviceKey key;

    bool isConnected;

public:
    explicit AbstractDeviceConnectionWatcher(QObject* parent = nullptr);
    ~AbstractDeviceConnectionWatcher();

    template <DeviceKey D>
    void setDevice(const QSharedPointer<typename deviceKeyTraits<D>::type>& dev) {

        using T = typename deviceKeyTraits<D>::type;
        static_assert(isDevice<T>::value, "Tipo device non valido");
        device = dev;
        key = D;

    }

protected:
    void setupTimers(int checkTimerMs);

    virtual void reconnectDevice() = 0;

public slots:
    void startWatcher();
    void stopWatcher();
    void checkConnection();

signals:
    void watcherStarted();
    void watcherStopped();
    void deviceDisconnected();
    void deviceConnected();

};

}

#endif // ABSTRACTDEVICECONNECTIONWATCHER_HPP
