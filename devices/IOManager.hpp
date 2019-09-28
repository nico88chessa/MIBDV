#ifndef IOMANAGER_HPP
#define IOMANAGER_HPP

#include <type_traits>

#include <QObject>
#include <QSharedPointer>
#include <QWeakPointer>

#include <Settings.hpp>
#include <Logger.hpp>
#include <Types.hpp>
#include <AbstractCN.hpp>
#include <AbstractPLC.hpp>
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

    QMap<DeviceKey, QSharedPointer<IAbstractDevice> > devices;

public:
    explicit IOManager(QObject *parent = nullptr);

    template <typename T>
    void addDevice(DeviceKey key, const QSharedPointer<T>& device) {

        traceEnter;
        static_assert (isPLC<T>::value || isCN<T>::value, "Device must be a CN or a PLC" );
        this->devices.insert(key, device);
        traceExit;

    }

    bool isConnected();

    bool connect();

private:

    template<typename T, std::enable_if_t<isCN<T>::value, int> = 0>
    bool setDigitalOutput(DeviceKey deviceKey, int channel, bool value) {

        traceEnter;

        using status = typename isCN<T>::statusType;
        using errorType = typename isCN<T>::errorType;

        if (!devices.contains(deviceKey)) {
            traceErr() << "Device" << Utils::getStringFromDeviceKey(deviceKey) << "non presente io IOManager";
            return false;
        }

        auto cn = devices.value(deviceKey).staticCast<AbstractCN<status, errorType>>();
        errorType e = cn->setDigitalOutput(channel, value);
        if (cn->isError(e)) {
            traceErr() << "Errore nel setting dell'uscita digitale del CN";
            return false;
        }

        traceExit;
        return true;

    }

    template<typename T, std::enable_if_t<isPLC<T>::value, int> = 0>
    bool setDigitalOutput(DeviceKey deviceKey, int channel, bool value) {

        traceEnter;

        using status = typename isPLC<T>::statusType;
        using errorType = typename isPLC<T>::errorType;

        if (!devices.contains(deviceKey)) {
            traceErr() << "Device" << Utils::getStringFromDeviceKey(deviceKey) << "non presente io IOManager";
            return false;
        }

        auto plc = devices.value(deviceKey).staticCast<AbstractPLC<status, errorType>>();
        errorType e = plc->setDigitalOutput(channel, value);
        if (plc->isError(e)) {
            traceErr() << "Errore nel setting dell'uscita digitale del PLC";
            return false;
        }

        traceExit;
        return true;

    }

public slots:
    bool setDigitalOutput(IOType type);
    bool unsetDigitalOutput(IOType type);

};

}

#endif // IOMANAGER_HPP
