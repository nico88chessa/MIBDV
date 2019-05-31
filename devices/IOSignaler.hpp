#ifndef IOSIGNALER_HPP
#define IOSIGNALER_HPP

#include <type_traits>

#include <QMap>
#include <QObject>
#include <QTimer>
#include <QMetaObject>

#include <Types.hpp>
#include <ConnectedDeviceInspector.hpp>
#include <DigitalInputValue.hpp>
#include <DigitalOutputValue.hpp>
#include <AnalogInputValue.hpp>


namespace PROGRAM_NAMESPACE {

class GalilCNStatusBean;
class GalilPLCStatusBean;

class IOSignaler : public QObject {
    Q_OBJECT

public:
    using Ptr = IOSignaler*;
    using ConstPtr = const IOSignaler*;

private:
    DigitalInputStatus digitalInputStatus;
    DigitalOutputStatus digitalOutputStatus;
    AnalogInputStatus analogInputStatus;

private:
    DigitalInputStatus digitalInputLastValues;
    bool isFirst;
    QList<QMetaObject::Connection> connections;

    QTimer refreshTimer;
    bool needSignaler;

public:

    explicit IOSignaler(QObject* parent = nullptr);

    ~IOSignaler();

    template <typename I>
    void subscribeInspector(const QSharedPointer<I>& inspector) {

        traceEnter;

        static_assert(isConnectedDeviceInspector<I>::value, "Il tipo deve essere un ConnectedDeviceInspector");
        using statusT = isConnectedDeviceInspector<I>::status;
        using typeT = isConnectedDeviceInspector<I>::type;

        static_assert(isCN<typeT>::value || isPLC<typeT>::value, "Il device deve essere un CN o un PLC");

        if (std::is_same<GalilCNController, typeT>::value) {
            auto&& c = connect(inspector.data(), &I::statusSignal, [&](const QVariant& status) {
                if (this != nullptr)
                    QMetaObject::invokeMethod(this, "updateStatus", Qt::QueuedConnection, Q_ARG(GalilCNStatusBean, qvariant_cast<GalilCNStatusBean>(status)));
            });
            connections.push_back(c);

        } else if (std::is_same<GalilPLCController, typeT>::value) {
            auto&& c = connect(inspector.data(), &I::statusSignal, [&](const QVariant& status) {
                if (this != nullptr)
                    QMetaObject::invokeMethod(this, "updateStatus", Qt::QueuedConnection, Q_ARG(GalilPLCStatusBean, qvariant_cast<GalilPLCStatusBean>(status)));
            });
            connections.push_back(c);
        }

        traceExit;

    }

    void unsubscribeAll() {
        for (auto&& c: connections)
            QObject::disconnect(c);
    }

private:
    void analizeIO();

private slots:
    void process();

public slots:
    void restartProcess();
    void startProcess();
    void stopProcess();

    void updateStatus(const GalilCNStatusBean& status);
    void updateStatus(const GalilPLCStatusBean& status);

signals:

    void powerOffSignal();
    void powerOnSignal();
    void cycleOffSignal();
    void cycleOnSignal();
    void markInProgressOnSignal();
    void markInProgressOffSignal();

    void statusSignal(
            const DigitalInputStatus& digitalInputs,
            const DigitalOutputStatus& digitalOutputs,
            const AnalogInputStatus& analogInputs);
    void processStartedSignal();
    void processStoppedSignal();

};

}

Q_DECLARE_METATYPE(PROGRAM_NAMESPACE::DigitalInputStatus)
Q_DECLARE_METATYPE(PROGRAM_NAMESPACE::DigitalOutputStatus)
Q_DECLARE_METATYPE(PROGRAM_NAMESPACE::AnalogInputStatus)

#endif // IOSIGNALER_HPP
