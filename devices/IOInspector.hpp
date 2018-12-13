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
#include <AbstractDevice.hpp>
#include <galil/GalilCNController.hpp>
#include <GalilCNStatusBean.hpp>
#include <galil/GalilPLCController.hpp>
#include <GalilPLCStatusBean.hpp>


namespace PROGRAM_NAMESPACE {

class IOInspector : public QObject {
    Q_OBJECT

public:
    using DigitalInputStatus = QMap<IOType, DigitalInputValue>;
    using DigitalOutputStatus = QMap<IOType, DigitalOutputValue>;
    using AnalogInputStatus = QMap<IOType, AnalogInputValue>;

private:
    DigitalInputStatus digitalInputStatus;
    DigitalOutputStatus digitalOutputStatus;
    AnalogInputStatus analogInputStatus;

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

private slots:
    void process();

public slots:
    void restartProcess();
    void startProcess();
    void stopProcess();
    void updateIOStatus(DeviceKey k, const QVariant& status);

signals:
//    void digitalInputsStateSignal(const QMap<DigitalInput, bool>& digitalInputs);
//    void digitalOutputsStateSignal(const QMap<DigitalOutput, bool>& digitalOutputs);
//    void analogInputsStateSignal(const QMap<AnalogInput, analogReal>& analogInputs);
    void statusSignal(
            const DigitalInputStatus& digitalInputs,
            const DigitalOutputStatus& digitalOutputs,
            const AnalogInputStatus& analogInputs);
    void processStartSignal();
    void processStopSignal();

};

}

Q_DECLARE_METATYPE(PROGRAM_NAMESPACE::IOInspector::DigitalInputStatus)
Q_DECLARE_METATYPE(PROGRAM_NAMESPACE::IOInspector::DigitalOutputStatus)
Q_DECLARE_METATYPE(PROGRAM_NAMESPACE::IOInspector::AnalogInputStatus)

#endif // IOINSPECTOR_HPP
