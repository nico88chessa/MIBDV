#include "IOManager.hpp"

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
        traceErr() << "Input" << Utils::getStringFromIOType(type) << "non e' presente";
        return false;
    }

    auto digitalOutput = digitalOutputs.value(type);

    switch (digitalOutput.getDevice()) {
        case DeviceKey::GALIL_CN: break;
        case DeviceKey::GALIL_PLC: break;
        default: break;
    }

    traceExit;
    return true;

}

bool IOManager::unsetDigitalOutput(IOType type) {

    traceEnter;

    if (!digitalOutputs.contains(type)) {
        traceErr() << "Input" << Utils::getStringFromIOType(type) << "non e' presente";
        return false;
    }

    auto digitalOutput = digitalOutputs.value(type);

    switch (digitalOutput.getDevice()) {
        case DeviceKey::GALIL_CN: break;
        case DeviceKey::GALIL_PLC: break;
        default: break;
    }

    traceExit;
    return true;

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
