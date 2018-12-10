#ifndef DEVICECONNECTIONWATCHER_HPP
#define DEVICECONNECTIONWATCHER_HPP

#include <QObject>
#include <QSharedPointer>
#include <QTimer>
#include <QThread>

#include <Types.hpp>

#include <AbstractDevice.hpp>

namespace PROGRAM_NAMESPACE {

class DeviceConnectionWatcher : public QObject {
    Q_OBJECT

public:
    using Ptr = DeviceConnectionWatcher*;
    using ConstPtr = const DeviceConnectionWatcher*;

protected:
    QSharedPointer<IAbstractDevice> device;
    QTimer connectionCheckTimer;
    DeviceKey key;

    bool isConnected;

public:
    explicit DeviceConnectionWatcher(QObject* parent = nullptr);
    ~DeviceConnectionWatcher();


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

#endif // DEVICECONNECTIONWATCHER_HPP
