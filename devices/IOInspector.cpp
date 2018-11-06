#include "IOInspector.hpp"
#include "Settings.hpp"

using namespace PROGRAM_NAMESPACE;

IOInspector::IOInspector(QObject *parent) : QObject(parent), needSignaler(false) {

    const Settings& settings = Settings::instance();
    digitalInputs = settings.getDigitalInputs();
    analogInputs = settings.getAnalogInputs();

    for (auto di: digitalInputs.values())
        digitalInputStatus.insert(di, false);

    for (auto ai: analogInputs.values())
        analogInputStatus.insert(ai, 0);

    Settings& s = Settings::instance();

    signalTimer.setInterval(s.getMachineIORefreshIntervalMs());
    connect(&signalTimer, &QTimer::timeout, this, &IOInspector::signalStatusTimeout);
    signalTimer.start();
}

void IOInspector::updateStatus(const GalilCNStatusBean& status) {

    traceEnter;

    // TODO NIC 05/11/2018 - gestire errori IO (per gli IO che prevedono l'errore)
    for (auto digIn: digitalInputStatus.keys()) {
        if (digIn.getDevice() == DeviceKey::GALIL_CN) {
            bool value = status.getDigitalInput(digIn.getChannel());
            digitalInputStatus.value(digIn, value);
        }
    }

    for (auto digOut: digitalOutputStatus.keys()) {
        if (digOut.getDevice() == DeviceKey::GALIL_CN) {
            bool value = status.getDigitalOutput(digOut.getChannel());
            digitalOutputStatus.value(digOut, value);
        }
    }

    traceExit;

}

void IOInspector::updateStatus(const GalilPLCStatusBean& status) {

    traceEnter;

    // TODO NIC 05/11/2018 - gestire errori IO (per gli IO che prevedono l'errore)
    for (auto digIn: digitalInputStatus.keys()) {
        if (digIn.getDevice() == DeviceKey::GALIL_PLC) {
            bool value = status.getDigitalInput(digIn.getChannel());
            digitalInputStatus.value(digIn, value);
        }
    }

    for (auto digOut: digitalOutputStatus.keys()) {
        if (digOut.getDevice() == DeviceKey::GALIL_PLC) {
            bool value = status.getDigitalOutput(digOut.getChannel());
            digitalOutputStatus.value(digOut, value);
        }
    }

    for (auto anIn: analogInputStatus.keys()) {
        if (anIn.getDevice() == DeviceKey::GALIL_PLC) {
            analogReal value = status.getAnalogInput(anIn.getChannel());
            analogInputStatus.value(anIn, value);
        }
    }

    traceExit;

}

void IOInspector::updateIOStatus(DeviceKey k, const QVariant& status) {

    traceEnter;

    switch (k) {
        case DeviceKey::GALIL_CN: this->updateStatus<DeviceKey::GALIL_CN>(status); break;
        case DeviceKey::GALIL_PLC: this->updateStatus<DeviceKey::GALIL_PLC>(status); break;
        case DeviceKey::NONE: break;
    }

    traceExit;

    return;

}

void IOInspector::signalStatusTimeout() {

    traceEnter;

    if (this->needSignaler) {
        emit digitalInputsStateSignal(digitalInputStatus);
        emit digitalOutputsStateSignal(digitalOutputStatus);
        emit analogInputsStateSignal(analogInputStatus);
    }
    needSignaler = false;

    traceExit;

}
