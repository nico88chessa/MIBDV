#include "IOManager.hpp"

#include <galil/GalilCNController.hpp>
#include <galil/GalilPLCController.hpp>

using namespace PROGRAM_NAMESPACE;

IOManager::IOManager(QObject *parent) : QObject(parent) {

    const Settings& settings = Settings::instance();
    digitalInputs = settings.getDigitalInputs();
    digitalOutputs = settings.getDigitalOutputs();
    analogInputs = settings.getAnalogInputs();

}

bool IOManager::setDigitalOutput(IOType type) {

    traceEnter;

    if (!digitalOutputs.contains(type)) {
        traceErr() << "Output" << Utils::getStringFromIOType(type) << "non e' presente";
        return false;
    }

    auto digitalOutput = digitalOutputs.value(type);
    DeviceKey deviceToUse = digitalOutput.getDevice();
    int channel = digitalOutput.getChannel();
    bool isInvertLogic = digitalOutputs.value(type).getInvertLogic();
    bool value = isInvertLogic ? false : true;

    traceInfo() << "Imposto uscita digitale" << channel << "del device" << Utils::getStringFromDeviceKey(deviceToUse) << "a" << value;

    bool result = false;
    switch (deviceToUse) {
        case DeviceKey::GALIL_CN:
            result = this->setDigitalOutput<deviceKeyTraits<DeviceKey::GALIL_CN>::type>(deviceToUse, channel, value);
            break;
        case DeviceKey::GALIL_PLC:
            result = this->setDigitalOutput<deviceKeyTraits<DeviceKey::GALIL_PLC>::type>(deviceToUse, channel, value);
            break;
        default: break;
    }

    traceExit;
    return result;

}

bool IOManager::unsetDigitalOutput(IOType type) {

    traceEnter;

    if (!digitalOutputs.contains(type)) {
        traceErr() << "Output" << Utils::getStringFromIOType(type) << "non e' presente";
        return false;
    }

    auto digitalOutput = digitalOutputs.value(type);
    DeviceKey deviceToUse = digitalOutput.getDevice();
    int channel = digitalOutput.getChannel();
    bool isInvertLogic = digitalOutputs.value(type).getInvertLogic();
    bool value = isInvertLogic ? true : false;

    traceInfo() << "Imposto uscita digitale" << channel << "del device" << Utils::getStringFromDeviceKey(deviceToUse) << "a" << value;

    bool result = false;
    switch (deviceToUse) {
        case DeviceKey::GALIL_CN:
            result = this->setDigitalOutput<deviceKeyTraits<DeviceKey::GALIL_CN>::type>(deviceToUse, channel, value);
            break;
        case DeviceKey::GALIL_PLC:
            result = this->setDigitalOutput<deviceKeyTraits<DeviceKey::GALIL_PLC>::type>(deviceToUse, channel, value);
            break;
        default: break;
    }

    traceExit;
    return result;

}

//void IOManager::setStatus(DeviceKey k, const QVariant& status) {

//    traceEnter;

//    switch (k) {
//        case DeviceKey::GALIL_CN: this->updateStatus<DeviceKey::GALIL_CN>(status); break;
//        case DeviceKey::GALIL_PLC: this->updateStatus<DeviceKey::GALIL_PLC>(status); break;
//        case DeviceKey::NONE: break;
//    }

//    traceExit;

//    return;

//}
