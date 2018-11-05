#ifndef IOMANAGER_HPP
#define IOMANAGER_HPP

#include <type_traits>

#include <QObject>
#include <QMap>
#include <QSharedPointer>
#include <QWeakPointer>

#include <AbstractDevice.hpp>
#include <DigitalInput.hpp>
#include <DigitalOutput.hpp>
#include <AnalogInput.hpp>
#include <CommonTraits.hpp>
#include <GalilCNController.hpp>
#include <GalilPLCController.hpp>
#include <Settings.hpp>
#include <Logger.hpp>



namespace PROGRAM_NAMESPACE {

class IOManager : public QObject {
    Q_OBJECT

private:
    QMap<IOType, DigitalInput> digitalInputs;
    QMap<IOType, DigitalOutput> digitalOutputs;
    QMap<IOType, AnalogInput> analogInputs;

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
    template<DeviceKey D>
    void updateStatus(const QVariant& status) {

        static_assert(deviceKeyTraits<D>::value, "Tipo di DeviceKey non valido");
        using deviceType = typename deviceKeyTraits<D>::type;
        static_assert(isDevice<deviceType>::value, "DeviceKey non e' associato ad alcun device noto");
        using statusType = typename isDevice<deviceType>::statusType;

        statusType s = status.value<statusType>();

    }

    bool setDigitalOutput(IOType type, bool value);

public slots:
    bool setDigitalOutput(IOType type);
    bool unsetDigitalOutput(IOType type);
    void setStatus(DeviceKey k, const QVariant& status);


signals:

};

}

#endif // IOMANAGER_HPP
