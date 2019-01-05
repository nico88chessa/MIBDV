#include "IOInspector.hpp"
#include "Settings.hpp"

using namespace PROGRAM_NAMESPACE;

IOInspector::IOInspector(QObject *parent) : QObject(parent), refreshTimer(this), needSignaler(false) {

    const Settings& settings = Settings::instance();
    DigitalInputSet dInMap = settings.getDigitalInputs();
    DigitalOutputSet dOutMap = settings.getDigitalOutputs();
    AnalogInputSet aInMap = settings.getAnalogInputs();

    for (auto dIn: dInMap.values()) {
        if (dIn.getChannel() == DIGITAL_INPUT_CHANNEL_NONE)
            continue;
        digitalInputStatus.insert(dIn.getElementType(), dIn);
    }

    for (auto dOut: dOutMap.values()) {
        if (dOut.getChannel() == DIGITAL_OUTPUT_CHANNEL_NONE)
            continue;
        digitalOutputStatus.insert(dOut.getElementType(), dOut);
    }

    for (auto aIn: aInMap.values()) {
        if (aIn.getChannel() == ANALOG_INPUT_CHANNEL_NONE)
            continue;
        analogInputStatus.insert(aIn.getElementType(), aIn);
    }

    int refreshTimeMs = settings.getMachineIORefreshIntervalMs();
    refreshTimer.setInterval(refreshTimeMs);

    connect(&refreshTimer, &QTimer::timeout, this, &IOInspector::process);

}

void IOInspector::updateStatus(const GalilCNStatusBean& status) {

    traceEnter;

    // TODO NIC 05/11/2018 - gestire errori IO (per gli IO che prevedono l'errore)
    for (auto dInKey: digitalInputStatus.keys()) {
        auto& dIn = digitalInputStatus[dInKey];
        if (dIn.getDevice() == DeviceKey::GALIL_CN) {
            bool value = status.getDigitalInput(dIn.getChannel());
            if (dIn.getInvertLogic())
                value = !value;
            dIn.setValue(value);
        }
    }

    for (auto dOutKey: digitalOutputStatus.keys()) {
        auto& dOut = digitalOutputStatus[dOutKey];
        if (dOut.getDevice() == DeviceKey::GALIL_CN) {
            bool value = status.getDigitalOutput(dOut.getChannel());
            if (dOut.getInvertLogic())
                value = !value;
            dOut.setValue(value);
        }
    }

    needSignaler = true;
    traceExit;

}

void IOInspector::updateStatus(const GalilPLCStatusBean& status) {

    traceEnter;

    // TODO NIC 05/11/2018 - gestire errori IO (per gli IO che prevedono l'errore)
    for (auto dInKey: digitalInputStatus.keys()) {
        auto& dIn = digitalInputStatus[dInKey];
        if (dIn.getDevice() == DeviceKey::GALIL_PLC) {
            bool value = status.getDigitalInput(dIn.getChannel());
            if (dIn.getInvertLogic())
                value = !value;
            dIn.setValue(value);
        }
    }

    for (auto dOutKey: digitalOutputStatus.keys()) {
        auto& dOut = digitalOutputStatus[dOutKey];
        if (dOut.getDevice() == DeviceKey::GALIL_PLC) {
            bool value = status.getDigitalOutput(dOut.getChannel());
            if (dOut.getInvertLogic())
                value = !value;
            dOut.setValue(value);
        }
    }

    for (auto aInKey: analogInputStatus.keys()) {
        auto& aIn = analogInputStatus[aInKey];
        if (aIn.getDevice() == DeviceKey::GALIL_PLC) {
            analogReal value = status.getAnalogInput(aIn.getChannel());
            value = value / GALIL_PLC_ANALOG_COUNT_MAX_VALUE * aIn.getGain() + aIn.getOffset();
            aIn.setValue(value);
        }
    }

    needSignaler = true;
    traceExit;

}

void IOInspector::process() {

    traceEnter;

    if (this->needSignaler) {
//        emit digitalInputsStateSignal(digitalInputStatus);
//        emit digitalOutputsStateSignal(digitalOutputStatus);
//        emit analogInputsStateSignal(analogInputStatus);
        emit statusSignal(digitalInputStatus, digitalOutputStatus, analogInputStatus);
    }
    needSignaler = false;

    traceExit;

}

void IOInspector::restartProcess() {
    traceEnter;
    // do nothing
    traceExit;
}

void IOInspector::startProcess() {

    traceEnter;
    refreshTimer.start();
    emit processStartSignal();
    traceExit;

}

void IOInspector::stopProcess() {

    traceEnter;

    if (refreshTimer.isActive())
        refreshTimer.stop();

    emit processStopSignal();
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
