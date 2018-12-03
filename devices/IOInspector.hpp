#ifndef IOINSPECTOR_HPP
#define IOINSPECTOR_HPP

#include <QObject>
#include <QMap>
#include <QTimer>

#include <configure.h>
#include <Constants.hpp>
#include <Logger.hpp>
#include <CommonTraits.hpp>
#include <DigitalInput.hpp>
#include <DigitalOutput.hpp>
#include <AnalogInput.hpp>
#include <AbstractDevice.hpp>
#include <galil/GalilCNController.hpp>
#include <GalilCNStatusBean.hpp>
#include <galil/GalilPLCController.hpp>
#include <GalilPLCStatusBean.hpp>


namespace PROGRAM_NAMESPACE {

class IOInspector : public QObject {
    Q_OBJECT

private:
    QMap<IOType, DigitalInput> digitalInputs;
    QMap<DigitalInput, bool> digitalInputStatus;

    QMap<IOType, DigitalInput> digitalOutputs;
    QMap<DigitalOutput, bool> digitalOutputStatus;

    QMap<IOType, AnalogInput> analogInputs;
    QMap<AnalogInput, analogReal> analogInputStatus;

    QTimer signalTimer;
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

signals:
    void digitalInputsStateSignal(const QMap<DigitalInput, bool>& digitalInputs);
    void digitalOutputsStateSignal(const QMap<DigitalOutput, bool>& digitalOutputs);
    void analogInputsStateSignal(const QMap<AnalogInput, analogReal>& analogInputs);

public slots:
    void updateIOStatus(DeviceKey k, const QVariant& status);
    void signalStatusTimeout();

};

}

#endif // IOINSPECTOR_HPP
