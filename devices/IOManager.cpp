#include "IOManager.hpp"

using namespace PROGRAM_NAMESPACE;

IOManager::IOManager(QObject *parent) : QObject(parent) {

    const Settings& settings = Settings::instance();
    digitalInputs = settings.getDigitalInputs();
    digitalOutputs = settings.getDigitalOutputs();
    analogInputs = settings.getAnalogInputs();

}

bool IOManager::setDigitalOutput(IOType type, bool value) {

    traceEnter;

    if (!digitalOutputs.contains(type)) {
        traceErr() << "Output" << Utils::getStringFromIOType(type) << "non e' presente";
        return false;
    }

    auto digitalOutput = digitalOutputs.value(type);

    DeviceKey deviceToUse = digitalOutput.getDevice();

    if (!devices.contains(deviceToUse)) {
        traceErr() << "Nessun device presente per l'output" << Utils::getStringFromIOType(type);
        return false;
    }

    QWeakPointer<IAbstractDevice> iDevice = devices.value(deviceToUse);
    IAbstractDevice::Ptr device = iDevice.data();

    if (!device) {
        traceErr() << "Il device dell'output" << Utils::getStringFromIOType(type) \
                   << "non e'' stato inizializzato o e'' stato eliminato";
        return false;
    }

    switch (deviceToUse) {
        case DeviceKey::GALIL_CN: {
            auto d = static_cast<GalilCNController::Ptr>(device);
            if (d->setDigitalOutput(digitalOutput.getChannel(), value) != G_NO_ERROR) {
                traceErr() << "Errore nel setting dell'uscita digitale del CN";
                return false;
            }
            }
            break;
        case DeviceKey::GALIL_PLC: {
            auto d = static_cast<GalilPLCController::Ptr>(device);
            if (d->setDigitalOutput(digitalOutput.getChannel(), value) != G_NO_ERROR) {
                traceErr() << "Errore nel setting dell'uscita digitale del PLC";
                return false;
            };
            }
            break;
        default: break;
    }

    traceExit;
    return true;

}

bool IOManager::setDigitalOutput(IOType type) {

    traceEnter;

    if (!digitalOutputs.contains(type)) {
        traceErr() << "Output" << Utils::getStringFromIOType(type) << "non e' presente";
        return false;
    }

    bool isInvertLogic = digitalOutputs.value(type).getInvertLogic();
    bool value = isInvertLogic ? false : true;
    bool result = this->setDigitalOutput(type, value);

    traceExit;
    return result;

}

bool IOManager::unsetDigitalOutput(IOType type) {

    traceEnter;

    if (!digitalOutputs.contains(type)) {
        traceErr() << "Output" << Utils::getStringFromIOType(type) << "non e' presente";
        return false;
    }

    bool isInvertLogic = digitalOutputs.value(type).getInvertLogic();
    bool value = isInvertLogic ? true : false;
    bool result = this->setDigitalOutput(type, value);

    traceExit;
    return result;

}

void IOManager::setStatus(DeviceKey k, const QVariant& status) {

    traceEnter;

    switch (k) {
        case DeviceKey::GALIL_CN: this->updateStatus<DeviceKey::GALIL_CN>(status); break;
        case DeviceKey::GALIL_PLC: this->updateStatus<DeviceKey::GALIL_PLC>(status); break;
        case DeviceKey::NONE: break;
    }

    traceExit;

    return;

}
