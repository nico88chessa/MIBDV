#ifndef DEVICECONNECTIONWATCHER_HPP
#define DEVICECONNECTIONWATCHER_HPP

#include <QObject>
#include <QWeakPointer>
#include <QTimer>
#include <QThread>

#include <Types.hpp>

#include <AbstractDevice.hpp>

namespace PROGRAM_NAMESPACE {

class ThreadNoEventLoopException : public std::exception {
    virtual const char* what() const noexcept {
        return "Current thread hasn't got any thread loop. Impossible to use DeviceConnectionWatcher.";
    }
};

class DeviceConnectionWatcher : public QObject {
    Q_OBJECT

public:
    using Ptr = DeviceConnectionWatcher*;
    using ConstPtr = const DeviceConnectionWatcher*;

protected:
    QWeakPointer<IAbstractDevice> device;
    QTimer connectionCheckTimer;
    bool isConnected;

public:
    explicit DeviceConnectionWatcher(QObject* parent = nullptr);
    ~DeviceConnectionWatcher();

    template <typename T>
    void setDevice(const QWeakPointer<T>& dev) {

        static_assert(isDevice<T>::value, "Tipo device non valido");
        device = dev;

    }

    void setupTimers(int checkTimerMs);

protected:
    void reconnectDevice();

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
