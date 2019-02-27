#ifndef IOINSPECTOR_HPP
#define IOINSPECTOR_HPP

#include <QObject>
#include <QMap>
#include <QTimer>

#include <configure.h>
#include <Constants.hpp>
#include <Logger.hpp>
#include <Types.hpp>
#include <DigitalInputValue.hpp>
#include <DigitalOutputValue.hpp>
#include <AnalogInputValue.hpp>
#include <devices/AbstractDevice.hpp>
#include <devices/motion/galil/GalilCNController.hpp>
#include <GalilCNStatusBean.hpp>
#include <devices/motion/galil/GalilPLCController.hpp>
#include <GalilPLCStatusBean.hpp>


namespace PROGRAM_NAMESPACE {

class IOInspector : public QObject {
    Q_OBJECT

private:
    DigitalInputStatus digitalInputStatus;
    DigitalOutputStatus digitalOutputStatus;
    AnalogInputStatus analogInputStatus;

private:
    DigitalInputStatus digitalInputLastValues;
    bool isFirst;

    QTimer refreshTimer;
    bool needSignaler;

public:
    explicit IOInspector(QObject *parent = nullptr);

private:
    template <DeviceKey D>
    void updateStatus(const QVariant& status) {

        traceEnter;

        static_assert(deviceKeyTraits<D>::value, "Tipo di DeviceKey non valido");
        using deviceType = typename deviceKeyTraits<D>::type;
        static_assert(isDevice<deviceType>::value, "DeviceKey non e' associato ad alcun device noto");
        using statusType = typename isDevice<deviceType>::statusType;

        statusType s = status.value<statusType>();
        needSignaler = true;
        this->updateStatus(s);

        traceExit;

    }

    void updateStatus(const GalilCNStatusBean& status);
    void updateStatus(const GalilPLCStatusBean& status);

    void analizeIO();

private slots:
    void process();

public slots:
    void restartProcess();
    void startProcess();
    void stopProcess();
    void updateIOStatus(DeviceKey k, const QVariant& status);

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
    void processStartSignal();
    void processStopSignal();

};

}

Q_DECLARE_METATYPE(PROGRAM_NAMESPACE::DigitalInputStatus)
Q_DECLARE_METATYPE(PROGRAM_NAMESPACE::DigitalOutputStatus)
Q_DECLARE_METATYPE(PROGRAM_NAMESPACE::AnalogInputStatus)

#endif // IOINSPECTOR_HPP
