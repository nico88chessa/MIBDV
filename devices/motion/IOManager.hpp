#ifndef IOMANAGER_HPP
#define IOMANAGER_HPP

#include <type_traits>

#include <QObject>
#include <QSharedPointer>
#include <QWeakPointer>

#include <Settings.hpp>
#include <Logger.hpp>
#include <Types.hpp>
#include <devices/motion/AbstractCN.hpp>
#include <devices/motion/AbstractPLC.hpp>
#include <DigitalInput.hpp>
#include <DigitalOutput.hpp>
#include <AnalogInput.hpp>
#include <Utils.hpp>

namespace PROGRAM_NAMESPACE {

using IOCommandErr = int;

static constexpr IOCommandErr IO_MANAGER_NO_ERR = PROGRAM_ERR_START_CODE + 1;

class IOManager : public QObject {
    Q_OBJECT

private:
    DigitalInputSet digitalInputs;
    DigitalOutputSet digitalOutputs;
    AnalogInputSet analogInputs;

    QMap<DeviceKey, QWeakPointer<IAbstractDevice> > devices;

public:
    explicit IOManager(QObject *parent = nullptr);

    template <typename T>
    void addDevice(DeviceKey key, const QSharedPointer<T>& device) {
        traceEnter;

        static_assert (isDevice<T>::value, "Device is not valid");
        static_assert (isPLC<T>::value || isCN<T>::value, "Device must be a CN or a PLC" );

        QWeakPointer<T> dev(device);
        this->devices.insert(key, dev);

        traceExit;
    }

private:

    template<typename T>
    bool setDigitalOutput(DeviceKey deviceKey, int channel, bool value) {

        traceEnter;

        static_assert(isPLC<T>::value || isCN<T>::value, "Il device deve essere un CN o un PLC");

        QWeakPointer<IAbstractDevice> iDevice = devices.value(deviceKey);
        IAbstractDevice::Ptr device = iDevice.data();

        if (!device) {
            traceErr() << "Il device dell'output" << Utils::getStringFromDeviceKey(deviceKey) \
                       << "non e'' stato inizializzato o e'' stato eliminato";
            return false;
        }

        if (isCN<T>::value) {
            using status = typename isCN<T>::statusType;
            using errorType = typename isCN<T>::errorType;
            auto d = static_cast<typename AbstractCN<status, errorType>::Ptr>(device);
            errorType e = d->setDigitalOutput(channel, value);
            if (d->isError(e)) {
                traceErr() << "Errore nel setting dell'uscita digitale del CN";
                return false;
            }

        } else if (isPLC<T>::value) {
            using status = typename isPLC<T>::statusType;
            using errorType = typename isPLC<T>::errorType;
            auto d = static_cast<typename AbstractPLC<status, errorType>::Ptr>(device);
            errorType e = d->setDigitalOutput(channel, value);
            if (d->isError(e)) {
                traceErr() << "Errore nel setting dell'uscita digitale del PLC";
                return false;
            }
        }

        traceExit;
        return true;

    }

public slots:
    bool setDigitalOutput(IOType type);
    bool unsetDigitalOutput(IOType type);


signals:
    void powerOffSignal();
    void powerOnSignal();
    void cycleOffSignal();
    void cycleOnSignal();
    void markInProgressOnSignal();
    void markInProgressOffSignal();

};

}

#endif // IOMANAGER_HPP
