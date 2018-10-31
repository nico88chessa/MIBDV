#ifndef IOMANAGER_HPP
#define IOMANAGER_HPP

#include <QObject>
#include <QMap>

#include <AbstractDevice.hpp>
#include <DigitalInput.hpp>
#include <DigitalOutput.hpp>
#include <AnalogInput.hpp>
#include <CommonTraits.hpp>
#include <type_traits>
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

public:
    explicit IOManager(QObject *parent = nullptr);

private:
    template<DeviceKey D>
    void updateStatus(const QVariant& status) {

        static_assert(deviceKeyTraits<D>::value, "Tipo di DeviceKey non valido");
        using deviceType = typename deviceKeyTraits<D>::type;
        static_assert(isDevice<deviceType>::value, "DeviceKey non e' associato ad alcun device noto");
        using statusType = typename isDevice<deviceType>::statusType;

        statusType s = status.value<statusType>();

    }

    bool setDigitalOutput(IOType type);
    bool unsetDigitalOutput(IOType type);


public slots:

    void setStatus(DeviceKey k, const QVariant& status);


signals:

};

}

#endif // IOMANAGER_HPP
